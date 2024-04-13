#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class RenderWindow {
    public:
        RenderWindow(const char* title, int window_width, int window_height) : title(title), width(window_width), height(window_height),
         window(nullptr), renderer(nullptr) {}

        bool CreateWindow () {
            window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
            if(window == NULL) {std::cout << "Window could not be created, Error: " << SDL_GetError() << "\n";}
            return window != nullptr;
        }
        bool CreateRenderer () {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL) {std::cout << "Renderer could not be created, Error:" << SDL_GetError() << "\n";}
            return renderer != nullptr;
        }
        void CleanWindow () {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
        }

        SDL_Window* getWindow() const {
            return window;
        }
        SDL_Renderer* getRenderer() const {
            return renderer;
        }


    
    
    private:
        const int width;
        const int height;
        std::string title;
        SDL_Window* window;
        SDL_Renderer* renderer;
};
bool is_mouse_in_button(int mouseX, int mouseY, int button[2][2]) {

    return (mouseX >= button[0][0] && mouseX <= button[1][0] &&
            mouseY >= button[0][1] && mouseY <= button[1][1]);

}

bool is_mouse_outside_window(int mouseX, int mouseY, const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
    return (mouseX < 0 || mouseX >= SCREEN_WIDTH || mouseY < 0 || mouseY >= SCREEN_HEIGHT);
}
/*
 return (mouseX >= button.top_left[0] && mouseX <= button.bottom_right[0] &&
            mouseY >= button.top_left[1] && mouseY <= button.bottom_right[1]);


*/
char checkWinner(char board[9]) {
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i * 3] != 'n' && board[i * 3] == board[i * 3 + 1] && board[i * 3] == board[i * 3 + 2]) {
            return board[i * 3]; // Return the winning player's mark
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (board[i] != 'n' && board[i] == board[i + 3] && board[i] == board[i + 6]) {
            return board[i]; // Return the winning player's mark
        }
    }

    // Check diagonals
    if (board[0] != 'n' && board[0] == board[4] && board[0] == board[8]) {
        return board[0]; // Return the winning player's mark
    }
    if (board[2] != 'n' && board[2] == board[4] && board[2] == board[6]) {
        return board[2]; // Return the winning player's mark
    }

    // No winner found
    return 'e';
}

SDL_Texture* create_texture(SDL_Renderer* renderer, const char* img_file_name) {
    SDL_Surface *imageSurface = IMG_Load(img_file_name); // Load image
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, imageSurface); // Create texture from surface
    SDL_FreeSurface(imageSurface); // Free the surface, we no longer need it
    return texture;
}
int main (int argv, char** args) {
    bool running = true;
    const int SCREEN_WIDTH = 800; 
    const int SCREEN_HEIGHT = 680;
    char player = 'x';
    bool player_toggle = true;

    const int CELL_WIDTH = SCREEN_WIDTH/3;
    const int CELL_HEIGHT = SCREEN_HEIGHT/3;
    const int OFFSET_X = CELL_WIDTH / 2;
    const int OFFSET_Y = CELL_HEIGHT / 2;

    const int HALF_X_WIDTH = 100;
    const int HALF_X_HEIGHT = 100;
    const int HALF_O_WIDTH = 100;
    const int HALF_O_HEIGHT = 100;


    // first parameter is topleft coordinates, second is bottom right coordinates
    int button1[2][2] = {{0, 0}, {SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3}};
    int button2[2][2] = {{SCREEN_WIDTH / 3, 0}, {2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3}};
    int button3[2][2] = {{2 * SCREEN_WIDTH / 3, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT / 3}};
    int button4[2][2] = {{0, SCREEN_HEIGHT / 3}, {SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3}};
    int button5[2][2] = {{SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3}, {2 * SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3}};
    int button6[2][2] = {{2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3}, {SCREEN_WIDTH, 2 * SCREEN_HEIGHT / 3}};
    int button7[2][2] = {{0, 2 * SCREEN_HEIGHT / 3}, {SCREEN_WIDTH / 3, SCREEN_HEIGHT}};
    int button8[2][2] = {{SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3}, {2 * SCREEN_WIDTH / 3, SCREEN_HEIGHT}};
    int button9[2][2] = {{2 * SCREEN_WIDTH / 3, 2 * SCREEN_HEIGHT / 3}, {SCREEN_WIDTH, SCREEN_HEIGHT}};

    // char board[3][3] = {{'n', 'n', 'n'},
    //                     {'n', 'n', 'n'},
    //                     {'n', 'n', 'n'}};
    char board[9] = {'n', 'n', 'n',
                     'n', 'n', 'n',
                     'n', 'n', 'n'};

    if(SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cout << "SDL FAILED TO INITIALISE VIDEO" << SDL_GetError() << "\n";
    }

    if(!(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "SDL FAILED TO INITIALISE IMAGE" << SDL_GetError() << "\n";
    }
    // initialises the class
    RenderWindow screen("Tic Tac Toe", SCREEN_WIDTH, SCREEN_HEIGHT);
    // usees the class functions 
    screen.CreateWindow();
    screen.CreateRenderer();

    SDL_Window* window = screen.getWindow();
    SDL_Renderer* renderer = screen.getRenderer();
    // create a surface/texture from these images and return it
    SDL_Texture* board_texture = create_texture(renderer, "res/board.png");
    SDL_Texture* o_texture = create_texture(renderer, "res/o_image.png");
    SDL_Texture* x_texture = create_texture(renderer, "res/x_image2.png");
    SDL_Texture* player1_texture = create_texture(renderer, "res/player1.png");
    SDL_Texture* player2_texture = create_texture(renderer, "res/player2.png");

    // Calculate the half width and half height of the X and O images
    

// Define cell positions with the center of each cell
    int cellPositions[9][2] = {
    {CELL_WIDTH / 2 - HALF_X_WIDTH, CELL_HEIGHT / 2 - HALF_X_HEIGHT},         // Cell 1
    {SCREEN_WIDTH / 3 + CELL_WIDTH / 2 - HALF_X_WIDTH, CELL_HEIGHT / 2 - HALF_X_HEIGHT}, // Cell 2
    {2 * SCREEN_WIDTH / 3 + CELL_WIDTH / 2 - HALF_X_WIDTH, CELL_HEIGHT / 2 - HALF_X_HEIGHT}, // Cell 3
    {CELL_WIDTH / 2 - HALF_X_WIDTH, SCREEN_HEIGHT / 3 + CELL_HEIGHT / 2 - HALF_X_HEIGHT}, // Cell 4
    {SCREEN_WIDTH / 3 + CELL_WIDTH / 2 - HALF_X_WIDTH, SCREEN_HEIGHT / 3 + CELL_HEIGHT / 2 - HALF_X_HEIGHT}, // Cell 5
    {2 * SCREEN_WIDTH / 3 + CELL_WIDTH / 2 - HALF_X_WIDTH, SCREEN_HEIGHT / 3 + CELL_HEIGHT / 2 - HALF_X_HEIGHT}, // Cell 6
    {CELL_WIDTH / 2 - HALF_X_WIDTH, 2 * SCREEN_HEIGHT / 3 + CELL_HEIGHT / 2 - HALF_X_HEIGHT}, // Cell 7
    {SCREEN_WIDTH / 3 + CELL_WIDTH / 2 - HALF_X_WIDTH, 2 * SCREEN_HEIGHT / 3 + CELL_HEIGHT / 2 - HALF_X_HEIGHT}, // Cell 8
    {2 * SCREEN_WIDTH / 3 + CELL_WIDTH / 2 - HALF_X_WIDTH, 2 * SCREEN_HEIGHT / 3 + CELL_HEIGHT / 2 - HALF_X_HEIGHT}  // Cell 9
    };



    SDL_SetRenderDrawColor(renderer, 80, 185, 80, 255);

    SDL_Rect piece_rect = {10, 10, 200, 200}; // x pos, y pos, width, height
    // SDL_Rect winscreen_rect = 
    //SDL_Rect x_rect
    while(running) {
        // declare variables
        int mouseX, mouseY;
        
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        
        // clears the target with the draw colour
        SDL_RenderClear(renderer);
        // define the events
        SDL_Event event;
        

        // sdl displays the events in a queue to be iterated through
        while (SDL_PollEvent(&event)) {
            // checks if the escape button has been quit
            if (event.type == SDL_QUIT) {
                
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                // Handle different key codes
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;

                    case SDLK_r:   
                        for (int i = 0; i < 9; ++i) {
                        board[i] = 'n';
                    }

            }    }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int clicked_cell = -1;
                if (is_mouse_in_button(mouseX, mouseY, button1) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if (board[0] == 'n') {
                        board[0] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 0;
                        player_toggle = !player_toggle;
                    }
                }
                else if (is_mouse_in_button(mouseX, mouseY, button2) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if (board[1] == 'n') {
                        board[1] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 1;
                        player_toggle = !player_toggle;
                    }
                }
                else if(is_mouse_in_button(mouseX, mouseY, button3) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if (board[2] == 'n') {
                        board[2] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 2;
                        player_toggle = !player_toggle;
                    }
                }
                else if(is_mouse_in_button(mouseX, mouseY, button4) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if(board[3] == 'n') {
                        board[3] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 3;
                        player_toggle = !player_toggle;

                    }
                }
                else if (is_mouse_in_button(mouseX, mouseY, button5) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if (board[4] == 'n') {
                        board[4] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 4;
                        player_toggle = !player_toggle;
                    }

                }
                else if (is_mouse_in_button(mouseX, mouseY, button6) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if (board[5] == 'n') {
                        board[5] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 5;
                        player_toggle = !player_toggle;
                    }
                }
                else if (is_mouse_in_button(mouseX, mouseY, button7) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if (board[6] == 'n') {
                        board[6] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 6;
                        player_toggle = !player_toggle;
                    }
                }
                else if (is_mouse_in_button(mouseX, mouseY, button8) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if (board[7] == 'n') {
                        board[7] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 7;
                        player_toggle = !player_toggle;
                    }
                }
                else if (is_mouse_in_button(mouseX, mouseY, button9) && !(is_mouse_outside_window(mouseX, mouseY, SCREEN_WIDTH, SCREEN_HEIGHT))) {
                    if (board[8] == 'n') {
                        board[8] = (player_toggle) ? 'x' : 'o'; // ternary operator
                        clicked_cell = 8;
                        player_toggle = !player_toggle;
                    }
                }


        }
        for(int index = 0; index < 9; index++) {
            if(board[index] == 'o') {
                piece_rect.x = cellPositions[index][0];
                piece_rect.y = cellPositions[index][1];
                SDL_RenderCopy(renderer, o_texture, NULL, &piece_rect); // Render 'x' for player 1


            }
            if(board[index] == 'x') {
                piece_rect.x = cellPositions[index][0];
                piece_rect.y = cellPositions[index][1];
                SDL_RenderCopy(renderer, x_texture, NULL, &piece_rect); // Render 'o' for player 2
            }
            
        }
        if(checkWinner(board) == 'o') {
            SDL_RenderCopy(renderer, player1_texture, NULL, NULL); // Render texture to screen
        }
        else if(checkWinner(board) == 'x') {
            SDL_RenderCopy(renderer, player2_texture, NULL, NULL); // Render texture to screen

        }
            //SDL_RenderCopy(renderer, o_texture, NULL, &piece_rect);
            
            // Check diagonals

        
        // if(player_toggle == true) {
        //     player = 'o'
        //     player_toggle = false
        // }
        is_mouse_in_button(mouseX, mouseY, button1);
        
        SDL_RenderCopy(renderer, board_texture, NULL, NULL); // Render texture to screen
        
        

        // updates the renderer after every loop
        SDL_RenderPresent(renderer);
    }
    }
    // to stop memory leaks and bugs once the app is quit
    // destroy window and renderer
    screen.CleanWindow();

    SDL_DestroyTexture(board_texture);
    SDL_DestroyTexture(o_texture);
    SDL_DestroyTexture(x_texture);
    SDL_DestroyTexture(player1_texture);
    SDL_DestroyTexture(player2_texture);

    SDL_Quit();
    std::cout << "Ended Succesfully";
    // to stop the terminal from closing
    return 0;
}


