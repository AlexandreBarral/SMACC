struct SsrFPatternForward1 : public smacc::SmaccState<SsrFPatternForward1, SS>
{
  using SmaccState::SmaccState;

  typedef smacc::transition<EvActionSucceeded<smacc::ClMoveBaseZ, OrNavigation>, SsrFPatternReturn1> reactions;

  static void onDefinition()
  {
    static_configure<OrNavigation, CbNavigateForward>(SS::ray_lenght_meters());
    static_configure<OrTool, CbToolStart>();
  }

  void onInitialize()
  {
    auto &superstate = this->context<SS>();
    ROS_INFO("[SsrFpattern] Fpattern rotate: SS current iteration: %d/%d", superstate.iteration_count, SS::total_iterations());
  }
};