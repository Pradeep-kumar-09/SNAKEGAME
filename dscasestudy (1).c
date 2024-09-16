#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

void setup();
void generateFruit();
void draw();
void input();
void logic();

struct Node {
    int x, y;
    struct Node* next;
};

int height = 20, width = 20;
int gameover, score;
int fruitx, fruity;
char direction;
struct Node* head;

void setup() {
    gameover = 0;
    score = 0;
    direction = 'd'; // Start with the snake moving to the right

    // Initialize the snake with a single node at the center
    head = (struct Node*)malloc(sizeof(struct Node));
    head->x = height / 2;
    head->y = width / 2;
    head->next = NULL;

    // Generate initial fruit location
    generateFruit();
}

void generateFruit() {
    fruitx = rand() % (height - 2) + 1; // Avoid generating fruit on the boundary
    fruity = rand() % (width - 2) + 1;  // Avoid generating fruit on the boundary
}

void draw() {
    system("cls");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                printf("#");
            } else {
                int isSnakeBody = 0;
                struct Node* current = head;
                while (current != NULL) {
                    if (current->x == i && current->y == j) {
                        printf("o");
                        isSnakeBody = 1;
                        break;
                    }
                    current = current->next;
                }
                if (!isSnakeBody) {
                    if (i == fruitx && j == fruity) {
                        printf("*");
                    } else {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }
    printf("score = %d\n", score);
    printf("press X to quit the game\n");
}

void input() {
    if (_kbhit()) {
        char new_direction = _getch();
        if ((new_direction == 'a' && direction != 'd') ||
            (new_direction == 's' && direction != 'w') ||
            (new_direction == 'd' && direction != 'a') ||
            (new_direction == 'w' && direction != 's')) {
            direction = new_direction;
        } else if (new_direction == 'x') {
            gameover = 1;
        }
    }
}

void logic() {
    // Move the snake in the current direction
    int new_x = head->x;
    int new_y = head->y;
    switch (direction) {
        case 'a':
            new_y--;
            break;
        case 's':
            new_x++;
            break;
        case 'd':
            new_y++;
            break;
        case 'w':
            new_x--;
            break;
    }

    // Create a new head node
    struct Node* new_head = (struct Node*)malloc(sizeof(struct Node));
    new_head->x = new_x;
    new_head->y = new_y;
    new_head->next = head;

    // Update the head of the snake
    head = new_head;

    // Check if the snake has eaten the fruit
    if (new_x == fruitx && new_y == fruity) {
        score += 10;
        generateFruit();
    } else {
        // If not, remove the tail of the snake (last node)
        struct Node* current = head;
        struct Node* prev = NULL;
        while (current->next != NULL) {
            prev = current;
            current = current->next;
        }
        prev->next = NULL;
        free(current);
    }

    // Check for collision with walls or itself
    if (new_x <= 0 || new_x >= height - 1 || new_y <= 0 || new_y >= width - 1) {
        gameover = 1;
    }
}

int main() {
    setup();
    while (!gameover) {
        draw();
        input();
        logic();
        Sleep(100); // Sleep for 100 milliseconds
    }

    // Free memory used by the snake nodes
    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
