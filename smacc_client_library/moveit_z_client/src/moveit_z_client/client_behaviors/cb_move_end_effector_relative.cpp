/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018-2020
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/

#include <moveit_z_client/client_behaviors/cb_move_end_effector_relative.h>
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <future>

namespace moveit_z_client
{
CbMoveEndEffectorRelative::CbMoveEndEffectorRelative()
{
    transform_.rotation.w = 1;
}

CbMoveEndEffectorRelative::CbMoveEndEffectorRelative(geometry_msgs::Transform transform)
{
}

void CbMoveEndEffectorRelative::onEntry()
{
    ROS_INFO_STREAM("[CbMoveEndEffectorRelative] Transform end effector pose relative: " << transform_);

    this->requiresClient(movegroupClient_);

    if (this->group_)
    {
        auto res = std::async(std::launch::async, [=] 
        {
                moveit::planning_interface::MoveGroupInterface move_group(*(this->group_));
                this->moveRelative(move_group, this->transform_);
        });
    }
    else
    {
        auto res = std::async(std::launch::async, [=] 
        {
            this->moveRelative(movegroupClient_->moveGroupClientInterface, this->transform_);
        });
    }
}

void CbMoveEndEffectorRelative::onExit()
{
}

void CbMoveEndEffectorRelative::moveRelative(moveit::planning_interface::MoveGroupInterface& moveGroupInterface, geometry_msgs::Transform &transformOffset)
{
    auto referenceStartPose = moveGroupInterface.getCurrentPose();
    tf::Quaternion currentOrientation;
    tf::quaternionMsgToTF(referenceStartPose.pose.orientation, currentOrientation);
    tf::Quaternion desiredRelativeRotation;

    tf::quaternionMsgToTF(transformOffset.rotation, desiredRelativeRotation);

    auto targetOrientation = desiredRelativeRotation * currentOrientation;

    geometry_msgs::PoseStamped targetObjectPose = referenceStartPose;

    tf::quaternionTFToMsg(targetOrientation, targetObjectPose.pose.orientation);
    targetObjectPose.pose.position.x += transformOffset.translation.x;
    targetObjectPose.pose.position.y += transformOffset.translation.y;
    targetObjectPose.pose.position.z += transformOffset.translation.z;

    moveGroupInterface.setPlanningTime(1.0);

    ROS_INFO_STREAM("[CbMoveEndEffectorRelative] Target End efector Pose: " << targetObjectPose);

    moveGroupInterface.setPoseTarget(targetObjectPose);
    moveGroupInterface.setPoseReferenceFrame("map");

    moveit::planning_interface::MoveGroupInterface::Plan computedMotionPlan;
    bool success = (moveGroupInterface.plan(computedMotionPlan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("CbMoveEndEffectorRelative", "Success Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");

    if (success)
    {
        auto executionResult = moveGroupInterface.execute(computedMotionPlan);

        if (executionResult == moveit_msgs::MoveItErrorCodes::SUCCESS)
        {
            ROS_INFO("[CbMoveEndEffectorRelative] motion execution succedded");
            movegroupClient_->postEventMotionExecutionSucceded();
        }
        else
        {
            ROS_INFO("[CbMoveEndEffectorRelative] motion execution failed");
            movegroupClient_->postEventMotionExecutionFailed();
        }
    }
    else
    {
        ROS_INFO("[CbMoveEndEffectorRelative] motion execution failed");
        movegroupClient_->postEventMotionExecutionFailed();
    }
}
} // namespace moveit_z_client