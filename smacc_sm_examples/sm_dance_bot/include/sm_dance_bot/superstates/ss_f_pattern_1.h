
namespace SS4
{
//forward declaration initial state
class SsrFPatternRotate1;
class SsrFPatternForward1;
class SsrFPatternReturn1;
class SsrFPatternRotate2;
class SsrFPatternForward2;

enum class TDirection {LEFT, RIGHT } ;

struct SsFPattern1 : smacc::SmaccState<SsFPattern1, SmDanceBot, SsrFPatternRotate1>
{
public:
    using SmaccState::SmaccState;

    typedef mpl::list<
                      // Keyboard events
                      sc::transition<smacc::EvKeyPressN<SbKeyboard>, StRotateDegrees4>,
                      sc::transition<EvKeyPressP<SbKeyboard>,StNavigateToWaypointsX>,
                       
                      // Error events
                      sc::transition<smacc::EvTopicMessageTimeout<LidarSensor>, StAcquireSensors>,
                      sc::transition<EvActionAborted<smacc::SmaccMoveBaseActionClient::Result>, StNavigateToWaypointsX>,

                      // Internal events
                      sc::custom_reaction<smacc::EvStateFinish<SsrFPatternForward2>>
            > reactions;


    float ray_lenght_meters;
    float pitch_lenght_meters;
    int iteration_count;
    int total_iterations;

    TDirection direction;

    void onInitialize()
    {
        this->ray_lenght_meters = 1;
        this->pitch_lenght_meters = 0.6;
        this->total_iterations = 3;
        this->direction = TDirection::LEFT;

        this->configure<KeyboardOrthogonal>(std::make_shared<SbKeyboard>());
    }

    sc::result react(const smacc::EvStateFinish<SsrFPatternForward2> &ev)
    {
        if (++iteration_count == total_iterations) // 1 == two times
        {
            this->throwFinishEvent();
        }

        return forward_event();
    }
};

//forward declaration for the superstate
using SS = SsFPattern1;
#include <sm_dance_bot/superstate_routines/f_pattern/ssr_fpattern_rotate_1.h>
#include <sm_dance_bot/superstate_routines/f_pattern/ssr_fpattern_forward_1.h>
#include <sm_dance_bot/superstate_routines/f_pattern/ssr_fpattern_return_1.h>
#include <sm_dance_bot/superstate_routines/f_pattern/ssr_fpattern_rotate_2.h>
#include <sm_dance_bot/superstate_routines/f_pattern/ssr_fpattern_forward_2.h>
} // namespace SS3