#include <smacc/smacc.h>
namespace sm_dance_bot
{
struct StRotateDegrees5 : smacc::SmaccState<StRotateDegrees5, MsDanceBotRunMode>
{
  using SmaccState::SmaccState;

  typedef mpl::list<
      // Expected event
      smacc::transition<EvActionSucceeded<smacc::ClMoveBaseZ, OrNavigation>, StNavigateToWaypointsX>,

      // Error events
      //smacc::transition<smacc::EvTopicMessageTimeout<CbLidarSensor>, StAcquireSensors>,
      smacc::transition<EvActionAborted<smacc::ClMoveBaseZ, OrNavigation>, StNavigateToWaypointsX>>
      reactions;

  static void onDefinition()
  {
    static_configure<OrNavigation, CbRotate>(/*30*/ -180);
    static_configure<OrTool, CbToolStop>();
    static_configure<OrObstaclePerception, CbLidarSensor>();
  }

  void onInitialize()
  {
  }
};
}