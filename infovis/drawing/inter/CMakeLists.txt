set(INTER_SOURCES
    Interactor.cpp
    Interactor3States.cpp
    InteractorEnterLeave.cpp
    InteractorIdle.cpp
    # InterMove.cpp
    KeyCodeInteractor.cpp
    InteractorManager.cpp
    Manager3State.cpp
    ManagerEnterLeave.cpp
    ManagerIdle.cpp
    DefaultManagerIdle.cpp
    Field.cpp
    EventCodes.cpp
    MouseHandler.cpp
    KeyboardHandler.cpp
    TimerHandler.cpp
    SysHandlers.cpp
)

add_library(liblite_inter STATIC ${INTER_SOURCES})
