#include <string>
#include <iostream>
#include <raylib.h>

struct Grid
{
    Rectangle rect;
    bool isAlive;
};

int main()
{
    int windowWidth = 600;
    int windowHeight = 700;
    int rowTot = 60;
    int colTot = 60;
    int cellSize = 10;
    bool run = false;

    int generation = 0;
    int alive = 0;

    InitWindow(windowWidth, windowHeight, "Game of Life");
    SetTargetFPS(20);

    Grid grid[rowTot][colTot];
    bool nextGen[rowTot][colTot];

    // Initialize grid
    for (int row = 0; row < rowTot; row++)
    {
        for (int col = 0; col < rowTot; col++)
        {
            grid[row][col].rect = {(float)row * cellSize, (float)col * cellSize, (float)cellSize, (float)cellSize};
            grid[row][col].isAlive = false;
            nextGen[row][col] = false;
        }
    }

    while (WindowShouldClose() == false)
    {
        // GoL rules
        if (run)
        {
            for (int row = 0; row < rowTot; row++)
            {
                for (int col = 0; col < colTot; col++)
                {
                    int count = 0;

                    for (int i = -1; i <= 1; i++)
                    {
                        for (int j = -1; j <= 1; j++)
                        {
                            if (i == 0 && j == 0)
                                continue;

                            int rx = row + i;
                            int ry = col + j;

                            if (rx >= 0 && ry >= 0 && rx < rowTot && ry < colTot)
                                // I'm in
                                count += grid[rx][ry].isAlive ? 1 : 0;
                        }
                    }

                    // Rule 1: Any live cell with fewer than two live neighbors dies, as if by underpopulation
                    // Rule 2: Any live cell with two or three live neighbors live on to the next generation
                    // Rule 3: Any live cell with more than three live neighbors dies, as if by overpopulation
                    // Rule 4: Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction
                    if (grid[row][col].isAlive)
                        // Rule 1 and 3
                        nextGen[row][col] = (count == 2 || count == 3);
                    else
                        nextGen[row][col] = (count == 3);
                }
            }

            // Update grid with the nextGen info
            for (int row = 0; row < rowTot; row++)
            {
                for (int col = 0; col < colTot; col++)
                {
                    grid[row][col].isAlive = nextGen[row][col];
                }
            }

            generation++;
        }

        // Input handling
        // ! this will break if you change the origin/start position of the grid to anything other than (0, 0)
        if (IsMouseButtonPressed(0))
        {
            Vector2 mousePos = (Vector2){GetMouseX(), GetMouseY()};

            grid[(int)mousePos.x / cellSize][(int)mousePos.y / cellSize].isAlive = !grid[(int)mousePos.x / cellSize][(int)mousePos.y / cellSize].isAlive;
            // std::cout << "x: " << (int)mousePos.x / 10 << ", " << "y: " << (int)mousePos.y / 10 << std::endl;
        }

        // Run (Start/Pause)
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            run = !run;

        // Reset
        if (IsKeyPressed(KEY_R))
            for (int row = 0; row < rowTot; row++)
                for (int col = 0; col < colTot; col++)
                {
                    grid[row][col].isAlive = false;
                    run = false;
                    generation = false;
                }

        // Drawing
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        // Grid
        for (int row = 0; row < rowTot; row++)
        {
            for (int col = 0; col < colTot; col++)
            {
                if (grid[row][col].isAlive)
                {
                    DrawRectangle(grid[row][col].rect.x, grid[row][col].rect.y, grid[row][col].rect.width, grid[row][col].rect.height, BLACK);
                    alive++;
                }
                else
                    DrawRectangleLines(grid[row][col].rect.x, grid[row][col].rect.y, grid[row][col].rect.width, grid[row][col].rect.height, GRAY);
            }
        }

        // UI
        // Generation
        std::string genStr = "Generation: " + std::to_string(generation);
        DrawText(genStr.c_str(), 10, GetScreenHeight() - 90, 20, GRAY);

        // Alive count
        std::string aliveStr = "Alive: " + std::to_string(alive);
        DrawText(aliveStr.c_str(), 10, GetScreenHeight() - 60, 20, GRAY);

        alive = 0;

        // Status
        if (run)
            DrawText("Running", GetScreenWidth() - 100, GetScreenHeight() - 90, 20, GREEN);
        else
            DrawText("Stopped", GetScreenWidth() - 100, GetScreenHeight() - 90, 20, RED);

        // Instructions
        DrawText("Space/Enter: Start/Pause", 10, GetScreenHeight() - 30, 20, GRAY);
        DrawText("R: Reset", 350, GetScreenHeight() - 30, 20, GRAY);
        DrawText("Esc: Quit", 480, GetScreenHeight() - 30, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}