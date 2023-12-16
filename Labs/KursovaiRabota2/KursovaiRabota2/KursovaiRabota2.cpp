#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>

#define MAX_SIZE 100

// Функция сохранения графа в файл
void saveGraphToFile(int graph[MAX_SIZE][MAX_SIZE], int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла\n");
        return;
    }

    fprintf(file, "%d\n", size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            fprintf(file, "%d ", graph[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    printf("\nГраф успешно сохранен в файл graph.txt\n\n");
}

// Функция сохранения результата алгоритма Прима в файл
void savePrimResultToFile(int parent[MAX_SIZE], int graph[MAX_SIZE][MAX_SIZE], int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла для сохранения результата\n");
        return;
    }

    fprintf(file, "Минимальное остовное дерево:\n");
    for (int i = 1; i < size; ++i) {
        fprintf(file, "Ребро: %d - %d, Вес: %d\n", parent[i], i, graph[i][parent[i]]);
    }

    fclose(file);

    printf("Результат успешно сохранен в файл result.txt\n\n");
}

// Функция для проверки существования файла
bool fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return true; // Файл существует
    }
    return false; // Файл не существует
}

// Загрузка графа из файла
void loadGraphFromFile(int graph[MAX_SIZE][MAX_SIZE], int* size, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("\nОшибка при открытии файла\n");
        return;
    }

    fscanf(file, "%d", size);
    for (int i = 0; i < *size; ++i) {
        for (int j = 0; j < *size; ++j) {
            fscanf(file, "%d", &graph[i][j]);
        }
    }

    fclose(file);
}

// Функция для заполнения неориентированного графа случайными числами
void fillUndirectedGraphRandom(int graph[MAX_SIZE][MAX_SIZE], int size)
{
    srand(time(NULL));

    // Заполнение графа нулями и единицами на главной диагонали
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                graph[i][j] = 0; // Главная диагональ остается нулевой
            }
            else {
                graph[i][j] = rand() % 2; // Заполнение ребер 0 или 1
                graph[j][i] = graph[i][j]; // Отражение изменений для неориентированного графа
            }
        }
    }

    // Замена единиц случайными числами
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i != j && graph[i][j] == 1) {
                graph[i][j] = rand() % 100; // Заполнение случайным числом вместо единицы
                graph[j][i] = graph[i][j]; // Отражение изменений для неориентированного графа
            }
        }
    }
}

// Вывод графа в консоль
void printGraph(int graph[MAX_SIZE][MAX_SIZE], int size)
{
    printf("\nГраф:\n");
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            printf("%3d ", graph[i][j]);
        }
        printf("\n");
    }
}

// Функция для нахождения несвязных вершин
int findUnconnectedNode(int parent[MAX_SIZE], int size) {
    for (int i = 0; i < size; ++i) {
        if (parent[i] == -1) {
            return i;
        }
    }
    return -1; // Если не найдено несвязанных вершин
}

// Функция выполнения алгоритма Прима для графа
void primAlgorithm(int graph[MAX_SIZE][MAX_SIZE], int size, int parent[MAX_SIZE]) {
    int minEdge[MAX_SIZE]; // Минимальные веса рёбер
    bool visited[MAX_SIZE]; // Посещали ли вершины

    // Инициализация значений
    for (int i = 0; i < size; ++i) {
        minEdge[i] = INT_MAX; // Установка начальных значений весов рёбер как максимальное значение
        visited[i] = false; // Начально ни одна вершина не посещена
        parent[i] = -1; // Начально нет родительской вершины
    }

    minEdge[0] = 0; // Стартовая вершина, вес ребра равен 0

    for (int count = 0; count < size - 1; ++count) {
        int minVertex = -1; // Минимальный индекс вершины
        for (int v = 0; v < size; ++v) {
            if (!visited[v] && (minVertex == -1 || minEdge[v] < minEdge[minVertex])) {
                minVertex = v; // Нахождение вершины с минимальным весом ребра
            }
        }

        visited[minVertex] = true; // Включаем найденную вершину в остовное дерево

        // Обновляем веса смежных вершин
        for (int v = 0; v < size; ++v) {
            if (graph[minVertex][v] != 0 && !visited[v] && graph[minVertex][v] < minEdge[v]) {
                parent[v] = minVertex;
                minEdge[v] = graph[minVertex][v];
            }
        }
    }

    // Вывод ребер минимального остовного дерева
    printf("\nМинимальное остовное дерево:\n");
    for (int i = 1; i < size; ++i) {
        if (parent[i] != -1) {
            printf("Ребро: %d - %d, Вес: %d\n", parent[i], i, graph[i][parent[i]]);
        }
        else {
            printf("Ребро: %d - %d, не существует\n", findUnconnectedNode(parent, size), i);
        }
    }
}

int main(void) {
    setlocale(LC_ALL, "RUS");

    int graph[MAX_SIZE][MAX_SIZE];
    int size, choice;
    char filename[50];
    int parent[MAX_SIZE];
    bool b = {};

    printf("Курсовая работа\n");
    printf("По курсу \"Логика и основы алгоритмизации в инженерных задачах\"\n");
    printf("На тему \"Реализация алгоритма Прима\"\n");
    printf("Выполнил: Городничев Максим Игоревич, группа: 22ВВВ3 (22ВВП2)\n\n");

    do {
        printf("Выберите действие:\n");
        printf("1. Загрузить неориентированный граф из файла\n");
        printf("2. Заполнить неориентированный граф компьютером\n");
        printf("0. Выйти из программы\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("\nВведите имя файла для загрузки: ");
            scanf("%s", filename);
            if (!fileExists(filename)) {
                printf("\nВведён несуществующий файл\n\n");
                break;
            }
            loadGraphFromFile(graph, &size, filename);
            printGraph(graph, size);
            primAlgorithm(graph, size, parent);
            saveGraphToFile(graph, size, "graph.txt");
            savePrimResultToFile(parent, graph, size, "result.txt");
            break;
        case 2:
            printf("\nВведите размер неориентированного графа: ");
            scanf("%d", &size);
            if (size <= 0) {
                printf("\nВведён неверный размер графа\n\n");
                break;
            }
            fillUndirectedGraphRandom(graph, size);
            printGraph(graph, size);
            primAlgorithm(graph, size, parent);
            saveGraphToFile(graph, size, "graph.txt");
            savePrimResultToFile(parent, graph, size, "result.txt");
            break;
        case 0:
            return 0; // Завершение программы при выборе "0"
        default:
            printf("\nСделан неверный выбор. Пожалуйста, выберите снова.\n\n");
            break;
        }
    } while (true);
}