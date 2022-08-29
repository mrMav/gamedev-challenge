set(HEADERS
${HEADERS}
ChallengeGame.h
Button.h
)

set(SOURCE
${SOURCE}
Button.cpp
ChallengeGame.cpp
Main.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)
