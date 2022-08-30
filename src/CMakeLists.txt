set(HEADERS
${HEADERS}
ChallengeGame.h
Button.h
GamePiece.h
)

set(SOURCE
${SOURCE}
GamePiece.cpp
Button.cpp
ChallengeGame.cpp
Main.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)
