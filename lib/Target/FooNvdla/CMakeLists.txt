
include_directories(.)
include_directories(include)

add_libonnc_src(
    CodeEmitVisitor.cpp
    FooNvdlaBackend.cpp
    Loadable.cpp
    NvDlaDefine.cpp
    NvDlaMeta.cpp
    NvDlaUtil.cpp
    NvDlaMemInfoPass.cpp
    NvDlaTaskSubmitPass.cpp
    NvDlaFileGenPass.cpp
    Config/NvFull.cpp
    TargetInfo/FooNvdlaTargetInfo.cpp
    TargetInfo/FooNvdlaTargetMemInfo.cpp
    Compute/NvDlaShuffle.cpp
    NvDlaIdentifyShufflePass.cpp
    PrintONNCIRPass.cpp)
