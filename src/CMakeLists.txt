set(HEADERS
${HEADERS}
ChallengeGame.h
)

set(SOURCE
${SOURCE}
ChallengeGame.cpp
Main.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)
