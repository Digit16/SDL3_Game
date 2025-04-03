#include "game.h"

int main(int argc, char *argv[]) {
    bool exit_status = EXIT_FAILURE;

    struct Game *game = NULL;

    srand((unsigned int)time(NULL));

    if (game_new(&game)) {
        game_run(game);

        exit_status = EXIT_SUCCESS;
    }

    game_free(&game);

    return exit_status;
}