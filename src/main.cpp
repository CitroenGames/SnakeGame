#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <string>
#include "SDL3/SDL.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float SEGMENT_SIZE = 20.0f;

void spawnFood(SDL_FRect& food) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, (SCREEN_WIDTH / (int)SEGMENT_SIZE) - 1);
    std::uniform_int_distribution<> disY(0, (SCREEN_HEIGHT / (int)SEGMENT_SIZE) - 1);

    food.x = disX(gen) * SEGMENT_SIZE;
    food.y = disY(gen) * SEGMENT_SIZE;
    food.w = SEGMENT_SIZE;
    food.h = SEGMENT_SIZE;
}

void resetGame(std::deque<SDL_FRect>& snake, Direction& direction, SDL_FRect& food) {
    snake.clear();
    snake.push_back({100, 300, SEGMENT_SIZE, SEGMENT_SIZE});
    snake.push_back({100 - SEGMENT_SIZE, 300, SEGMENT_SIZE, SEGMENT_SIZE});
    snake.push_back({100 - (2 * SEGMENT_SIZE), 300, SEGMENT_SIZE, SEGMENT_SIZE});
    direction = Direction::RIGHT;
    spawnFood(food);
}

#if defined(_WIN32) || defined(_WIN64)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main(int argc, char* argv[]) {
#endif

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SnakeGame - Score: 0",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0
    );

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Game variables
    std::deque<SDL_FRect> snake;
    Direction current_direction;
    SDL_FRect food;
    resetGame(snake, current_direction, food);
    
    Direction next_direction = current_direction;
    bool game_over = false;
    bool snake_grows = false;
    Uint32 last_update_time = 0;
    const Uint32 update_interval = 100;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (game_over && e.key.scancode == SDL_SCANCODE_R) {
                    resetGame(snake, current_direction, food);
                    next_direction = current_direction;
                    game_over = false;
                } else {
                    switch (e.key.scancode) {
                        case SDL_SCANCODE_UP:
                        case SDL_SCANCODE_W:
                            if(current_direction != Direction::DOWN) next_direction = Direction::UP;
                            break;
                        case SDL_SCANCODE_DOWN:
                        case SDL_SCANCODE_S:
                            if(current_direction != Direction::UP) next_direction = Direction::DOWN;
                            break;
                        case SDL_SCANCODE_LEFT:
                        case SDL_SCANCODE_A:
                            if(current_direction != Direction::RIGHT) next_direction = Direction::LEFT;
                            break;
                        case SDL_SCANCODE_RIGHT:
                        case SDL_SCANCODE_D:
                            if(current_direction != Direction::LEFT) next_direction = Direction::RIGHT;
                            break;
                        default: break;
                    }
                }
            }
        }

        if (!game_over) {
            Uint32 current_time = SDL_GetTicks();
            if (current_time - last_update_time > update_interval) {
                current_direction = next_direction;
                SDL_FRect new_head = snake.front();
                switch (current_direction) {
                    case Direction::UP: new_head.y -= SEGMENT_SIZE; break;
                    case Direction::DOWN: new_head.y += SEGMENT_SIZE; break;
                    case Direction::LEFT: new_head.x -= SEGMENT_SIZE; break;
                    case Direction::RIGHT: new_head.x += SEGMENT_SIZE; break;
                }

                // Screen wrapping
                if (new_head.x < 0) new_head.x = SCREEN_WIDTH - SEGMENT_SIZE;
                else if (new_head.x >= SCREEN_WIDTH) new_head.x = 0;
                if (new_head.y < 0) new_head.y = SCREEN_HEIGHT - SEGMENT_SIZE;
                else if (new_head.y >= SCREEN_HEIGHT) new_head.y = 0;

                // Self collision (use direct position comparison for grid-based game)
                for (size_t i = 1; i < snake.size(); ++i) {
                    if (new_head.x == snake[i].x && new_head.y == snake[i].y) {
                        game_over = true;
                        break;
                    }
                }

                if (!game_over) {
                    snake.push_front(new_head);

                    // Food collision (use direct position comparison for grid-based game)
                    if (new_head.x == food.x && new_head.y == food.y) {
                        snake_grows = true;
                        spawnFood(food);
                        std::string title = "SnakeGame - Score: " + std::to_string(snake.size() - 3);
                        SDL_SetWindowTitle(window, title.c_str());
                    }

                    if (snake_grows) {
                        snake_grows = false;
                    } else {
                        snake.pop_back();
                    }
                }
                last_update_time = current_time;
            }
        }

        // --- Rendering ---
        SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Red for food
        SDL_RenderFillRect(renderer, &food);

        if (game_over) {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Red for game over
        } else {
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // Green for snake
        }
        for (const auto& segment : snake) {
            SDL_RenderFillRect(renderer, &segment);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}