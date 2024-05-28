#include "raylib.h"
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

const int screenWidth = 1200;
const int screenHeight = 800;
const int gridSize = 32;

struct Tile {
    int id;
    Rectangle textureRect;
};

struct TileSet {
    Texture2D texture;
    std::vector<Tile> tiles;

    void Load(const char* texturePath, int tileWidth, int tileHeight) {
        texture = LoadTexture(texturePath);
        int columns = texture.width / tileWidth;
        int rows = texture.height / tileHeight;
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {
                Tile tile;
                tile.id = y * columns + x;
                tile.textureRect = { (float)x * tileWidth, (float)y * tileHeight, (float)tileWidth, (float)tileHeight };
                tiles.push_back(tile);
            }
        }
    }

    void Unload() {
        UnloadTexture(texture);
    }
};

struct TileMap {
    int width, height;
    std::vector<int> data;

    TileMap(int width, int height) : width(width), height(height) {
        data.resize(width * height, -1);
    }

    void SetTile(int x, int y, int tileId) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            data[y * width + x] = tileId;
        }
    }

    int GetTile(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return data[y * width + x];
        }
        return -1;
    }
};

std::vector<TileSet> tileSets;
TileSet* currentTileSet = nullptr;
int selectedTileId = -1;
Vector2 offset = { 0, 0 };
float zoom = 1.0f;
bool menuVisible = false;

void DrawGrid(int width, int height, int gridSize);
void DrawMenu();
void PlaceTile(int mouseX, int mouseY, TileMap& map);
void RemoveTile(int mouseX, int mouseY, TileMap& map);
void DrawTiles(const TileMap& map);
void HandleCamera();
void DrawTileSetUI(const TileSet& tileSet, float tileSize, Vector2 position, int& selectedTileId);
std::string LoadFileDialog();

int main() {
    InitWindow(screenWidth, screenHeight, "2D Tilemap Editor");

    SetTargetFPS(60);

    TileMap tileMap(20, 15);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_TAB)) menuVisible = !menuVisible;

        HandleCamera();

        Vector2 mousePosition = GetMousePosition();
        Vector2 worldPosition = { (mousePosition.x - offset.x) / zoom, (mousePosition.y - offset.y) / zoom };

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaceTile(worldPosition.x, worldPosition.y, tileMap);
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            RemoveTile(worldPosition.x, worldPosition.y, tileMap);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(Camera2D { offset, { 0, 0 }, 0, zoom });
        DrawGrid(screenWidth, screenHeight, gridSize);
        DrawTiles(tileMap);
        EndMode2D();

        int gridX = (int)worldPosition.x / gridSize * gridSize;
        int gridY = (int)worldPosition.y / gridSize * gridSize;
        DrawRectangleLines(gridX, gridY, gridSize, gridSize, RED);

        if (menuVisible) {
            DrawMenu();
        }

        EndDrawing();
    }

    for (auto& tileSet : tileSets) {
        tileSet.Unload();
    }

    CloseWindow();
    return 0;
}

void DrawGrid(int width, int height, int gridSize) {
    for (int x = 0; x < width; x += gridSize) {
        DrawLine(x, 0, x, height, LIGHTGRAY);
    }
    for (int y = 0; y < height; y += gridSize) {
        DrawLine(0, y, width, y, LIGHTGRAY);
    }
}

void DrawMenu() {
    int menuWidth = 200;
    DrawRectangle(screenWidth - menuWidth, 0, menuWidth, screenHeight, Fade(SKYBLUE, 0.5f));
    DrawText("Menu", screenWidth - menuWidth + 10, 10, 20, BLACK);

    DrawText("Load Tileset", screenWidth - menuWidth + 10, 40, 20, BLACK);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && GetMouseX() > screenWidth - menuWidth + 10 && GetMouseX() < screenWidth - menuWidth + 150 &&
        GetMouseY() > 40 && GetMouseY() < 60) {
        std::string tilesetPath = LoadFileDialog();
        if (!tilesetPath.empty()) {
            TileSet tileSet;
            tileSet.Load(tilesetPath.c_str(), 32, 32);
            tileSets.push_back(tileSet);
            currentTileSet = &tileSets.back();
        }
    }

    if (currentTileSet != nullptr) {
        DrawTileSetUI(*currentTileSet, gridSize, { (float)screenWidth - menuWidth + 10, 70 }, selectedTileId);
    }
}

void PlaceTile(int mouseX, int mouseY, TileMap& map) {
    int gridX = mouseX / gridSize * gridSize;
    int gridY = mouseY / gridSize * gridSize;
    if (selectedTileId != -1) {
        map.SetTile(gridX / gridSize, gridY / gridSize, selectedTileId);
    }
}

void RemoveTile(int mouseX, int mouseY, TileMap& map) {
    int gridX = mouseX / gridSize * gridSize;
    int gridY = mouseY / gridSize * gridSize;
    map.SetTile(gridX / gridSize, gridY / gridSize, -1);
}

void DrawTiles(const TileMap& map) {
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            int tileId = map.GetTile(x, y);
            if (tileId != -1 && currentTileSet != nullptr) {
                Tile tile = currentTileSet->tiles[tileId];
                DrawTextureRec(currentTileSet->texture, tile.textureRect, { (float)x * gridSize, (float)y * gridSize }, WHITE);
            }
        }
    }
}

void HandleCamera() {
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        offset.x += GetMouseDelta().x;
        offset.y += GetMouseDelta().y;
    }
    zoom += GetMouseWheelMove() * 0.1f;
    if (zoom < 0.1f) zoom = 0.1f;
}

void DrawTileSetUI(const TileSet& tileSet, float tileSize, Vector2 position, int& selectedTileId) {
    for (int i = 0; i < tileSet.tiles.size(); i++) {
        Tile tile = tileSet.tiles[i];
        Vector2 tilePosition = { position.x + (i % 5) * (tileSize + 10), position.y + (i / 5) * (tileSize + 10) };
        DrawTextureRec(tileSet.texture, tile.textureRect, tilePosition, WHITE);
        if (CheckCollisionPointRec(GetMousePosition(), { tilePosition.x, tilePosition.y, tileSize, tileSize })) {
            DrawRectangleLines(tilePosition.x, tilePosition.y, tileSize, tileSize, RED);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedTileId = tile.id;
            }
        }
    }
}

std::string LoadFileDialog() {
    std::string path;
    std::cout << "Enter the path to the tileset image: ";
    std::cin >> path;
    return path;
}
