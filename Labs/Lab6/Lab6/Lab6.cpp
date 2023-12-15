#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <conio.h>

using namespace std;

// Создание матрицы смежности 
vector<vector<int>> generateAndPrintMatrix(int num_vertices, double edge_probability) {
	vector<vector<int>> adjacency_matrix(num_vertices + 1, vector<int>(num_vertices + 1, 0));
	int num_edges = 0;

	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = i + 1; j <= num_vertices; ++j) {
			if (static_cast<double>(rand()) / RAND_MAX < edge_probability) {
				adjacency_matrix[i][j] = 1;
				adjacency_matrix[j][i] = 1;
				num_edges++;
			}
		}
	}

	cout << "\nМатрица смежности:\n";
	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = 1; j <= num_vertices; ++j) {
			cout << adjacency_matrix[i][j] << ' ';
		}
		cout << endl;
	}
	return adjacency_matrix;
}

// Преорозование матрицы смежности в список 
vector<vector<int>> matrixToList(const vector<vector<int>>& matrix) {
	int num_vertices = matrix.size() - 1;
	vector<vector<int>> adjacency_list(num_vertices + 1);

	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = 1; j <= num_vertices; ++j) {
			if (matrix[i][j] == 1) {
				adjacency_list[i].push_back(j);
			}
		}
	}
	return adjacency_list;
}

// Вывод списка смежности 
void printList(const vector<vector<int>>& list) {
	cout << "\nСписок смежности:\n";
	for (int i = 1; i < list.size(); ++i) {
		cout << i << ": ";
		for (const int& j : list[i]) {
			cout << j << ' ';
		}
		cout << endl;
	}
	cout << "\n";
}

// Вывод матрицы
void printMatrix(const vector<vector<int>>& matrix) {
	cout << "\nМатрица смежности:\n";
	for (int i = 1; i < matrix.size(); i++) {
		for (int j = 1; j < matrix[i].size(); j++) {
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}
}

//Отождествления вершин
void identifyVertices(const vector < vector < int>>& originalMatrix, const vector < vector < int>>& originalList, int vertex1, int vertex2, vector < vector < int>>& resultMatrix, vector < vector < int>>& resultList) {
	// Создание копий исходной матрицы и списка смежности
	resultMatrix = originalMatrix;
	resultList = originalList;

	int n = resultMatrix.size();

	// Обновление копии матрицы смежности
	for (int i = 0; i < n; ++i) {
		if (resultMatrix[vertex2][i]) {
			resultMatrix[vertex1][i] = 1;
			resultMatrix[i][vertex1] = 1;
		}
		resultMatrix[vertex2][i] = 0;
		resultMatrix[i][vertex2] = 0;
	}

	// Обновление копии списка смежности
	for (int neighbor : resultList[vertex2]) {
		if (find(resultList[vertex1].begin(), resultList[vertex1].end(), neighbor) == resultList[vertex1].end() && neighbor != vertex1) {
			resultList[vertex1].push_back(neighbor);
		}
	}

	// Удаление vertex2 из всех списков смежности в копии
	for (auto& neighbors : resultList) {
		auto it = remove(neighbors.begin(), neighbors.end(), vertex2);
		neighbors.erase(it, neighbors.end());
		for (auto& neighbor : neighbors) {
			if (neighbor > vertex2) {
				--neighbor;
			}
		}
	}

	// Удаление vertex2 из копии матрицы
	resultMatrix.erase(resultMatrix.begin() + vertex2);
	for (auto& row : resultMatrix) {
		row.erase(row.begin() + vertex2);
	}

	// Удаление списка смежности для vertex2 и обновление последующих индексов в копии
	resultList.erase(resultList.begin() + vertex2);
}


//Стягивания ребра
void contractEdge(const vector<vector<int>>& originalMatrix, const vector<vector<int>>& originalList, int vertex1, int vertex2, vector<vector<int>>&
	resultMatrix, vector<vector<int>>& resultList) {
	resultMatrix = originalMatrix;
	resultList = originalList;

	int n = resultMatrix.size();

	// Проверка на наличие ребра между vertex1 и vertex2
	if (!resultMatrix[vertex1][vertex2] && !resultMatrix[vertex2][vertex1]) {
		printf("\nНет ребра между %d и %d для стягивания.\n", vertex1, vertex2);
		return;
	}

	// Объединение вершин в копии матрицы смежности
	for (int i = 0; i < n; ++i) {
		if (resultMatrix[vertex2][i]) {
			resultMatrix[vertex1][i] = 1;
			resultMatrix[i][vertex1] = 1;
		}
		resultMatrix[vertex2][i] = 0;
		resultMatrix[i][vertex2] = 0;
	}
	resultMatrix[vertex1][vertex1] = 0;

	// Объединение списков смежности
	for (int neighbor : resultList[vertex2]) {
		if (find(resultList[vertex1].begin(), resultList[vertex1].end(), neighbor) == resultList[vertex1].end() && neighbor != vertex1) {
			resultList[vertex1].push_back(neighbor);
		}
	}

	// Удаление vertex2 из всех списков смежности в копии
	for (auto& neighbors : resultList) {
		auto it = remove(neighbors.begin(), neighbors.end(), vertex2);
		neighbors.erase(it, neighbors.end());
	}

	// Удаление vertex2 из копии матрицы и списка смежности
	resultMatrix.erase(resultMatrix.begin() + vertex2);
	for (auto& row : resultMatrix) {
		row.erase(row.begin() + vertex2);
	}
	resultList.erase(resultList.begin() + vertex2);
}

//Расщепление вершины
void splitVertex(vector < vector < int>>& originalMatrix, vector < vector < int>>& originalList, int vertex, vector < vector < int>>& resultMatrix, vector < vector < int>>& resultList) {
	int n = originalMatrix.size();

	// Создаем новую матрицу смежности и новый список смежности
	resultMatrix.resize(n + 1, vector<int>(n + 1, 0));
	resultList.resize(n + 1);

	// Копируем исходную матрицу смежности и список смежности в новые структуры
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			resultMatrix[i][j] = originalMatrix[i][j];
		}
		resultList[i] = originalList[i];
	}

	// Копируем вершину, которую отождествляем
	for (int i = 0; i < n; ++i) {
		resultMatrix[i][n] = originalMatrix[i][vertex];
		resultMatrix[n][i] = originalMatrix[vertex][i];
	}

	// Добавляем новую вершину в список смежности
	for (int i = 0; i < n; ++i) {
		if (i != vertex) {
			resultList[i].push_back(n);
		}
	}

	// Добавляем новую вершину в конец списка смежности
	for (int i = 0; i < originalList[vertex].size(); ++i) {
		resultList[n].push_back(originalList[vertex][i]);

	}
	resultList[n].push_back(vertex);
	resultList[vertex].push_back(n);
	resultMatrix[vertex][n] = 1;
	resultMatrix[n][vertex] = 1;
}

//Объединения
vector < vector < int>> mergeGraphs(const vector < vector < int>>& graph1, const vector < vector < int>>& graph2) {
	int maxSize = max(graph1.size(), graph2.size());

	// Создание новой матрицы смежности для объединенного графа
	vector < vector < int>> mergedGraph(maxSize, vector<int>(maxSize, 0));

	// Копирование связей из первого графа
	for (int i = 0; i < graph1.size(); ++i) {
		for (int j = 0; j < graph1[i].size(); ++j) {
			mergedGraph[i][j] = graph1[i][j];
		}
	}

	// Добавление или перезапись связей из второго графа
	for (int i = 0; i < graph2.size(); ++i) {
		for (int j = 0; j < graph2[i].size(); ++j) {
			if (graph2[i][j] > 0) {
				mergedGraph[i][j] = graph2[i][j];
			}
		}
	}
	return mergedGraph;
}

//Пересечения
vector < vector < int>> intersectGraphs(const vector < vector < int>>& graph1, const vector < vector < int>>& graph2) {
	int size = min(graph1.size(), graph2.size());
	vector < vector < int>> intersectionGraph(size, vector<int>(size, 0));

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			// Элемент в новой матрице будет 1, если соответствующие элементы обеих матриц равны 1
			intersectionGraph[i][j] = (graph1[i][j] && graph2[i][j]);
		}
	}
	return intersectionGraph;
}

//Кольцевой суммы
vector < vector < int>> adaptiveRingSumGraphs(const vector < vector < int>>& graph1, const vector < vector < int>>& graph2) {
	int maxSize = max(graph1.size(), graph2.size());
	vector < vector < int>> ringSumGraph(maxSize, vector<int>(maxSize, 0));

	for (int i = 0; i < maxSize; ++i) {
		for (int j = 0; j < maxSize; ++j) {
			int val1 = (i < graph1.size() && j < graph1.size()) ? graph1[i][j] : 0;
			int val2 = (i < graph2.size() && j < graph2.size()) ? graph2[i][j] : 0;
			ringSumGraph[i][j] = val1 != val2;
		}
	}
	return ringSumGraph;
}

//Декартова произведения
vector < vector < int>> cartesianProductGraphs(const vector < vector < int>>& graph1, const vector < vector < int>>& graph2) {
	int n1 = graph1.size() - 1; // Учитываем, что размеры матриц начинаются с 1
	int n2 = graph2.size() - 1;
	int productSize = n1 * n2;
	vector < vector < int>> productGraph(productSize + 1, vector<int>(productSize + 1, 0)); // +1 для корректного размера

	for (int i1 = 1; i1 <= n1; ++i1) {
		for (int i2 = 1; i2 <= n2; ++i2) {
			for (int j1 = 1; j1 <= n1; ++j1) {
				for (int j2 = 1; j2 <= n2; ++j2) {
					int v1 = (i1 - 1) * n2 + i2; // Пересчитываем индексы
					int v2 = (j1 - 1) * n2 + j2;
					if (i1 == j1 && graph2[i2][j2]) {
						productGraph[v1][v2] = 1;
					}
					if (i2 == j2 && graph1[i1][j1]) {
						productGraph[v1][v2] = 1;
					}
				}
			}
		}
	}
	return productGraph;
}

int main()
{
	setlocale(LC_ALL, "RUS");
	int num_vertices, operation;
	int graph_choice, vertex1, vertex2;
	double edge_probability = 0.5;

	vector<vector<int>> matrix1, matrix2, matrixCopy;
	vector<vector<int>> list1, list2, listCopy;

	srand(static_cast<unsigned int>(time(nullptr)));

	while (true) {

		cout << "Меню:\n1) Сгенерировать графы\n2) Отождествление и cтягивание\n3) Расщепление вершины\n4) Бинарные операции\n0) Выход\nВыберите операцию:\n";

		operation = _getch() - '0'; // получаем символ и конвертируем его в число

		if (operation == 0) {
			break;
		}

		switch (operation) {
			//генерация матриц
		case 1: {
			cout << "\n";
			cout << "Введите количество вершин в первом графе (> 0): ";
			cin >> num_vertices;
			if (num_vertices <= 0) {
				cout << "Введено неверное значение\n";
				continue;
			}

			cout << "\n";
			cout << "Первый граф:\n";
			matrix1 = generateAndPrintMatrix(num_vertices, edge_probability);
			list1 = matrixToList(matrix1);
			printList(list1);

			cout << "\n";
			cout << "Введите количество вершин во втором графе (> 0): ";
			cin >> num_vertices;
			if (num_vertices <= 0) {
				cout << "Введено неверное значение\n";
				continue;
			}

			cout << "Второй граф:\n";
			matrix2 = generateAndPrintMatrix(num_vertices, edge_probability);
			list2 = matrixToList(matrix2);
			printList(list2);
			break;
		}

		case 2: {
			cout << "\n";
			cout << "Введите номер графа для выполнения операции (1 или 2): ";
			cin >> graph_choice;
			if (graph_choice <= 0) {
				cout << "Введено неверное значение!";
				continue;
			}

			cout << "\n";
			cout << "Введите номера двух вершин (например, 1 3): ";

			cin >> vertex1 >> vertex2;
			if (vertex1 == vertex2) {
				cout << "Введено неверное значение!";
				continue;
			}

			if (graph_choice == 1) {

				cout << "Отождествление:\n";
				identifyVertices(matrix1, list1, vertex1, vertex2, matrixCopy, listCopy);

				printMatrix(matrixCopy);
				printList(matrixToList(matrixCopy));

				cout << "Стягивание:\n";
				contractEdge(matrix1, list1, vertex1, vertex2, matrixCopy, listCopy);

				printMatrix(matrixCopy);
				printList(matrixToList(matrixCopy));
			}
			else if (graph_choice == 2) {

				cout << "Отождествление:\n";
				identifyVertices(matrix2, list2, vertex1, vertex2, matrixCopy, listCopy);

				printMatrix(matrixCopy);
				printList(matrixToList(matrixCopy));

				cout << "Стягивание:\n";
				contractEdge(matrix2, list2, vertex1, vertex2, matrixCopy, listCopy);

				printMatrix(matrixCopy);
				printList(matrixToList(matrixCopy));
			}
			break;
		}

		case 3: {
			cout << "\n";
			cout << "Введите номер графа для выполнения операции (1 или 2): ";
			cin >> graph_choice;
			if (graph_choice <= 0) {
				cout << "Введено неверное значение!\n";
				continue;
			}
			cout << "Введите номер вершины для расщепления: ";
			cin >> vertex1;
			if (vertex1 <= 0) {
				cout << "Введено неверное значение!";
				continue;
			}
			if (graph_choice == 1) {
				splitVertex(matrix1, list1, vertex1, matrixCopy, listCopy);
				printMatrix(matrixCopy);
				printList(matrixToList(matrixCopy));
			}
			else if (graph_choice == 2) {
				splitVertex(matrix2, list2, vertex1, matrixCopy, listCopy);
				printMatrix(matrixCopy);
				printList(matrixToList(matrixCopy));
			}
			break;
		}
		case 4: {
			vector<vector<int>> mergedGraph = mergeGraphs(matrix1, matrix2);
			cout << "\nОбъединение: \n";
			printMatrix(mergedGraph);


			vector<vector<int>> intersectionGraph = intersectGraphs(matrix1, matrix2);
			cout << "\nПересечение: \n";
			printMatrix(intersectionGraph);

			vector<vector<int>> ringSumGraph = adaptiveRingSumGraphs(matrix1, matrix2);
			cout << "\nКольцевая сумма: \n";
			printMatrix(ringSumGraph);

			vector<vector<int>> cartesianProductGraph = cartesianProductGraphs(matrix1, matrix2);
			cout << "\nДекартово произведение: \n";
			printMatrix(cartesianProductGraph);
			break;
		}
		default:
			cout << "Неверный выбор операции\n";
			break;
		}
	}
	return 0;
}