#pragma once

#include <sm_dance_bot/clients/cl_updatable_publisher.h>
#include <smacc/smacc_orthogonal.h>

namespace sm_dance_bot
{
class OrUpdatablePublisher : public smacc::Orthogonal
{
public:
    virtual void onInitialize() override
    {
        auto publisherClient_ = this->createClient<OrUpdatablePublisher, sm_dance_bot::ClUpdatableStringPublisher>();
        publisherClient_->topicName = "/updatable_string_publisher_out";
        publisherClient_->initialize();
    }
};
} // namespace sm_dance_bot