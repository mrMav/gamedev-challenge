set(HEADERS
${HEADERS}
Button.h
ChallengeGame.h
GamePiece.h
)

set(SOURCE
${SOURCE}
Button.cpp
ChallengeGame.cpp
GamePiece.cpp
Main.cpp
)

target_sources(${PROJECT_NAME} 
    PUBLIC ${HEADERS}
    PUBLIC ${SOURCE}
)
