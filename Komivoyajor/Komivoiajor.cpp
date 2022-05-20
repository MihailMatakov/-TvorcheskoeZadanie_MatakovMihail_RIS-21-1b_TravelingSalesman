#include<SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <queue>
#include <list>
using namespace std;
using namespace sf;

template <class T>
class Graph
{
public:
	vector<T> vertexList;
	vector<vector<int>> adjMatrix;
	int maxSize;
	queue<T> VertsQueue;
	vector<int> labelList;
	int rec = INT_MAX;
	std::list<int> path;
public:
	Graph(const int& size);
	Graph() : Graph(7) {};
	void showAdj();
	bool isEmpty();
	bool isFull();
	int GetVertPos(const T&);
	int GetAmountVerts();
	void insertVertex(const T& ver);
	int GetAmountEdges();
	int GetPath();
	int GetWeight(const T&, const T&);
	void InsertEdge(const T& vertex1, const T& vertex2, int weight = 1);
	vector<T> GetNbrs(const T&);
	void Print();
	bool AllVisited(vector<bool>& visitedVerts);
	void FillLabels(T& startVertex);
	void Draw();
	void ADD(const T& ver,int val);
	void DelVertex(T& ver);
	void DelE(const T& vertex1, const T& vertex2);
	std::vector<int> getNBRS(const int& vert);
	void opop(const int& start,const int& current, std::list<int>& path, vector<bool>& visitedVerts);
	void eval(const int& start);
};

template<class T>
int Graph<T>::GetPath()
{

	return this->path;
};

template<class T>
void Graph<T>::eval(const int& start) // Начало обхода создание вектора посещёных вершин
{
	std::list<int> path;
	path.push_back(start);
	std::vector<bool> vis(this->vertexList.size(),false);
	vis[start] = true;
	this->opop(start,start,path,vis);
}
template<class T>
std::vector<int> Graph<T>::getNBRS(const int& vert)
{
	std::list<int> res;

	for (int j = 0; j < this->vertexList.size(); ++j) 
	{
		if (this->adjMatrix[vert][j]) 
		{
			res.push_back(j);
		}
	}



	return std::vector<int>(res.begin(),res.end());
}

template<class T>  // Обход графа и поиск кратчайшего пути
void Graph<T>::opop(const int& start,const int& current, std::list<int>& path, vector<bool>& visitedVerts)
{
	if (this->AllVisited(visitedVerts) )
	{
		int min = 0;
		if (!this->adjMatrix[current][start]) 
		{
			return;
		}


		std::vector<int> p(path.begin(), path.end());
		for (int i = 0; i < p.size() - 1; ++i) 
		{
			min += this->adjMatrix[p[i]][p[i + 1]];
			/*std::cout << this->vertexList[p[i]] << ' ';*/
		}
		min+=this->adjMatrix[current][start];
		std::cout << std::endl;
		if (rec > min)
		{
			rec = min;
			this->path = path;
		}

		std::cout << std::endl;
	}

	std::vector<int> nbrs = this->getNBRS(current);

	for (int& i : nbrs)
	{
		if (!visitedVerts[i])
		{
			vector<bool> cpy_vis(visitedVerts.begin(), visitedVerts.end());
			list<int> path_cpy(path.begin(), path.end());
			cpy_vis[i] = true;
			path_cpy.push_back(i);
			opop(start,i, path_cpy, cpy_vis);
		}
	};

};


template<class T>// Добавление  вершины
inline void Graph<T>::ADD(const T& ver,int val)
{
	this->maxSize = val;
	this->vertexList.push_back(ver);
};

template<class T>// Удаление вершины
inline void Graph<T>::DelVertex(T& ver)
{
	if (maxSize == 0 || GetVertPos(ver) == -1)
	{
		cout << "Вершину удалить невозможно" << endl;
		return;
	}
	for (int j = 0; j < vertexList.size(); ++j)
		if (vertexList[j] == ver)
		{
			maxSize--;
			vertexList.erase(vertexList.begin() + j);
			adjMatrix.erase(adjMatrix.begin() + j);
			for (int i = 0; i < adjMatrix.size(); i++)
				adjMatrix[i].erase(adjMatrix[i].begin() + j);
		}
}


template<class T>// Удаление ребра
void Graph<T>::DelE(const T& vertex1, const T& vertex2) {
	if (GetVertPos(vertex1) != (-1) && this->GetVertPos(vertex2) != (-1)) {
		int vertPos1 = GetVertPos(vertex1);
		int vertPos2 = GetVertPos(vertex2);
			this->adjMatrix[vertPos1][vertPos2] = 0;
			this->adjMatrix[vertPos2][vertPos1] = 0;
	}
	else
	{
		cout << "Как минимум 1 вершини не хватает нет в графе " << endl;
		return;
	}
};


bool isMove = false;
template<class T>
inline void Graph<T>::Draw()// Визуализация графа
{
	const float pi = 3.14159f;
	const int Width = 800;
	const int Height = 600;
	bool f=false;
	RenderWindow window(VideoMode(Width, Height), " Graph ", Style::Close);
	while (window.isOpen())
	{
		Event event;
		window.clear(Color(227, 217, 202));
		CircleShape circle(200.f);
		RectangleShape Add(Vector2f(180.f,100.f));
		RectangleShape AddE(Vector2f(180.f, 100.f));
		RectangleShape Del(Vector2f(180.f, 100.f));
		RectangleShape DelE(Vector2f(180.f, 100.f));
		RectangleShape Put(Vector2f(180.f, 100.f));
		Add.setFillColor(Color(235, 213, 213));
		Add.setOutlineThickness(3);
		Add.setOutlineColor(Color::Black);
		AddE.setPosition(0.f, 105.f);
		AddE.setFillColor(Color(235, 213, 213));
		AddE.setOutlineThickness(3);
		AddE.setOutlineColor(Color::Black);
		Put.setPosition(0.f, 420.f);
		Put.setFillColor(Color(235, 213, 213));
		Put.setOutlineThickness(3);
		Put.setOutlineColor(Color::Black);
		Del.setPosition(0.f, 210.f);
		Del.setFillColor(Color(235, 213, 213));
		Del.setOutlineThickness(3);
		Del.setOutlineColor(Color::Black);
		DelE.setPosition(0.f, 315.f);
		DelE.setFillColor(Color(235, 213, 213));
		DelE.setOutlineThickness(3);
		DelE.setOutlineColor(Color::Black);
		Vector2i mousePoz = Mouse::getPosition(window);
		Font font;
		font.loadFromFile("timesnewroman.ttf");
		Text pathmin_text;
		pathmin_text.setPosition(10.f, 540.f);
		pathmin_text.setFont(font);
		pathmin_text.setFillColor(Color::Black);
		pathmin_text.setCharacterSize(20);
		pathmin_text.setString("Path min:  ");
		Text hmin_text;
		
		hmin_text.setPosition(90.f, 540.f);
		hmin_text.setFont(font);
		hmin_text.setFillColor(Color::Black);
		hmin_text.setCharacterSize(20);
		window.draw(hmin_text);

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
			{
				if(event.key.code==Mouse::Left)
					if (Add.getGlobalBounds().contains(mousePoz.x, mousePoz.y))
					{
						string h;
						cout << "Введите вершину для ввода";
						cin >> h;
						this->ADD(h,(vertexList.size()+1));
						this->Print();
					}
			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
					if (AddE.getGlobalBounds().contains(mousePoz.x, mousePoz.y))
					{
						string x,y ;
						int h;
						cout << "Введите вершины и вес";
						cin >> x >>y >>h;
						this->InsertEdge(x, y, h);
						this->Print();
					}
			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
					if (Del.getGlobalBounds().contains(mousePoz.x, mousePoz.y))
					{
						string ver;
						cout << "Введите вершину для удаления ";
						cin >> ver;
						this->DelVertex(ver);
						this->Print();
					}
			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
					if (Put.getGlobalBounds().contains(mousePoz.x, mousePoz.y))
					{
						string ver;
						cout << "Введите вершину с которой начнётся обход ";
						cin >> ver;
						this->eval(this->GetVertPos(ver));
						cout << this->rec << endl;
						if (rec == INT_MAX)
						{
							rec = 0;
						}
						this->Print();
						for(int& i : this->path)
						cout << this->vertexList[i]
							<< ' ';
						f = true;
					}
				
			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
					if (DelE.getGlobalBounds().contains(mousePoz.x, mousePoz.y))
					{
						string x,y;
						cout << "Введите Вершины  графф между которыми надо удалить  ";
						cin >> x >> y;
						this->DelE(x, y);
						this->Print();
					}
			}
		}
		
		Text name_text;
		name_text.setPosition(10.f, 20.f);
		name_text.setFont(font);
		name_text.setFillColor(Color::Black);
		name_text.setCharacterSize(20);
		name_text.setString("Insert vertex ");

		Text name1_text;
		name1_text.setPosition(10.f, 120.f);
		name1_text.setFont(font);
		name1_text.setFillColor(Color::Black);
		name1_text.setCharacterSize(20);
		name1_text.setString("Insert edge ");

		Text name2_text;
		name2_text.setPosition(10.f, 220.f);
		name2_text.setFont(font);
		name2_text.setFillColor(Color::Black);
		name2_text.setCharacterSize(20);
		name2_text.setString("Delete vertex ");

		Text name3_text;
		name3_text.setPosition(10.f, 320.f);
		name3_text.setFont(font);
		name3_text.setFillColor(Color::Black);
		name3_text.setCharacterSize(20);
		name3_text.setString("Delete edge ");

		Text name4_text;
		name4_text.setPosition(10.f, 435.f);
		name4_text.setFont(font);
		name4_text.setFillColor(Color::Black);
		name4_text.setCharacterSize(20);
		name4_text.setString("Path ");

		circle.setFillColor(sf::Color(234, 138, 138));
		circle.setRadius(40.f);
		circle.setPointCount(100);
		circle.setOrigin(40.f, 40.f);
		circle.setPosition(window.getSize().x / 2, window.getSize().y / 2);
		
		Text text;
		text.setFont(font);
		text.setCharacterSize(100);
		text.setFillColor(Color::Black);
		vector<CircleShape> circles;
		vector<Text> textes;
		vector<Text> edges;
		text.setCharacterSize(20);
		for (int i = 0; i < maxSize; i++)
		{
			text.setString(vertexList[i]);
			FloatRect textRect = text.getLocalBounds();
			text.setOrigin(textRect.left + textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);
			text.setPosition(window.getSize().x / 2 + 160 * cos(i * 2 * 3.14 / maxSize), window.getSize().y / 2 + 160 * sin(i * 2 * 3.14 / maxSize));
			circle.setPosition(window.getSize().x / 2 + 160 * cos(i * 2 * 3.14 / maxSize), window.getSize().y / 2 + 160 * sin(i * 2 * 3.14 / maxSize));
			circles.push_back(circle);
			textes.push_back(text);
		}
		for (int i = 0; i < maxSize; i++)
			for (int j = 0; j < maxSize; j++)
				if (adjMatrix[i][j] != 0)
				{
					Vertex line[] =
					{
						Vertex(Vector2f(circles[i].getPosition().x, circles[i].getPosition().y)),
						Vertex(Vector2f(circles[j].getPosition().x, circles[j].getPosition().y))
					};
					window.draw(line, 2, Lines);
					text.setString(to_string(adjMatrix[i][j]));
					FloatRect textRect = text.getLocalBounds();
					text.setOrigin(textRect.left + textRect.width / 2.0f,
						textRect.top + textRect.height / 2.0f);
					text.setPosition((circles[i].getPosition().x + circles[j].getPosition().x) / 2, (circles[i].getPosition().y + circles[j].getPosition().y) / 2);
					edges.push_back(text);
				}
		for (int i = 0; i < edges.size(); i++)
		{
			window.draw(edges[i]);
		}
		for (int i = 0; i < maxSize; i++)
		{
			window.draw(circles[i]);
			window.draw(Add);
			window.draw(AddE);
			window.draw(Del);
			window.draw(DelE);
			window.draw(Put);
			if (f == true)
			{
				hmin_text.setString(to_string(rec));
				window.draw(hmin_text);
			};
			window.draw(name4_text);
			window.draw(name_text);
			window.draw(name1_text);
			window.draw(name2_text);
			window.draw(name3_text);
			window.draw(pathmin_text);
			window.draw(textes[i]);
		}
		window.display();
	}
}
template <class T>
void Graph<T>::FillLabels(T& startVertex)
{
	for (int i = 0, size = vertexList.size(); i < size; ++i)
		labelList[i] = 1000000;
	int pos = GetVertPos(startVertex);
	labelList[pos] = 0;
};
template <class T> 
bool Graph<T>::AllVisited(vector<bool>& visitedVerts)// Проверка посещены ли все вершины
{
	bool flag = true;
	for (int i = 0; i < this->vertexList.size(); i++)
		if (visitedVerts[i] != true)
			flag = false;
	return flag;
};
template<class T>
inline Graph<T>::Graph(const int& size)
{
	this->labelList = vector<int>(size);
	this->maxSize = size;
	this->adjMatrix = vector<vector<int>>(size, vector<int>(size));
	for (int i = 0; i < this->maxSize; ++i)
	{
		for (int j = 0; j < this->maxSize; ++j)
		{
			this->adjMatrix[i][j] = 0;
		};
	};
};

template<class T>
inline void Graph<T>::showAdj() 
{
	for (int i = 0; i < this->maxSize; ++i)
	{
		for (int j = 0; j < this->maxSize; ++j)
		{
			cout << this->adjMatrix[i][j] << ' ';
		};
		cout << endl;
	}
};

template<class T>
inline bool Graph<T>::isEmpty()
{
	return this->vertexList.size() == 0;
};

template<class T>// Проверка все ли названия вершин введены
inline bool Graph<T>::isFull()
{
	return this->vertexList.size() == this->maxSize;
};

template<class T>//Определяет индекс на котором стоит вершина
inline int Graph<T>::GetVertPos(const T& v)
{
	for (int i = 0; i < this->vertexList.size(); ++i)
	{
		if (this->vertexList[i] == v)
		{
			return i;
		}
	}
	return -1;
};

template<class T>
inline int Graph<T>::GetAmountVerts()
{
	return this->vertexList.size();
};

template<class T>// Добавление вершины
inline void Graph<T>::insertVertex(const T& vert)
{
	bool f = false;
	for(int i=0;i<vertexList.size();++i)
		if (this->vertexList[i] == vert)
		{
			cout << "Данная вершина уже имеется в графе ";
			f = true;
	    }
	if (f == false)
	{
		if (this->isFull())
		{
			cout << "Невозможно добавить вершину." << endl;
			return;
		};
		this->vertexList.push_back(vert);
	};
};

template<class T>
int Graph<T>::GetAmountEdges()
{
	int amount = 0;
	if (!this->IsEmpty())
	{
		for (int i = 0; i < this->vertexList.size(); ++i)
		{
			for (int j = 0; j < this->vertexList.size(); ++j)
			{
				if (this->adjMatrix[i][j] != 0)
				{
					amount++;
				}
			}
		}
	}
	return amount;
};

template<class T>
inline int Graph<T>::GetWeight(const T& v1, const T& v2)
{
	if (this->isEmpty())
	{
		return 0;
	};

	int v1_p = this->GetVertPos(v1);
	int v2_p = this->GetVertPos(v2);

	if (v1_p == -1 || v2_p == -1)
	{
		cout << "Одного из узлов в графе не существует." << endl;
		return 0;
	};

	return this->adjMatrix[v1_p][v2_p];
};
template<class T>
std::vector<T> Graph<T>::GetNbrs(const T& vertex) {
	std::vector<T> nbrsList;
	int pos = this->GetVertPos(vertex);
	if (pos != -1)
	{
		for (int i = 0; i < this->vertexList.size(); ++i)
		{
			if (this->adjMatrix[pos][i] != 0)
			{
				nbrsList.push_back(this->vertexList[i]);
			}
		}
	}
	return nbrsList;
};
template<class T>// Добавляет ребро между вершинами
void Graph<T>::InsertEdge(const T& vertex1, const T& vertex2, int weight) 
{
	if(weight<0)
	{
		weight = abs(weight);
	}
	if (GetVertPos(vertex1) != (-1) && this->GetVertPos(vertex2) != (-1)) {
		int vertPos1 = GetVertPos(vertex1);
		int vertPos2 = GetVertPos(vertex2);
		if (vertPos1!= vertPos2)
		{
			if (this->adjMatrix[vertPos1][vertPos2] != 0)
			{
				cout << "Ребро между вершинами уже есть" << endl;
				return;
			}
			else
			{
				this->adjMatrix[vertPos1][vertPos2] = weight;
				this->adjMatrix[vertPos2][vertPos1] = weight;
			}
		}
	}
	else
	{
		cout << "Как минимум 1 вершини не хватает нет в графе " << endl;
		return;
	}
};

template<class T>// Печать таблицы смежности
void Graph<T>::Print() {
	if (!this->isEmpty())
	{
		cout << "Матрица смежности графа: " << endl;

		cout << "-  ";
		for (int i = 0; i < vertexList.size(); ++i)
		{
			cout << vertexList[i] << "  ";
		}
		cout << endl;

		for (int i = 0; i < this->vertexList.size(); ++i)
		{
			cout << this->vertexList[i] << " ";
			for (int j = 0; j < this->vertexList.size(); ++j)
			{
				cout << " " << this->adjMatrix[i][j] << " ";
			}
			cout << endl;
		}
	}
	else {
		cout << "Граф пуст " << endl;
	}
}




int main()
{
	int E;
	setlocale(LC_ALL, "Russian");
	int V, ves; string ver, source, target;
	cout << " колическто вершин: "; cin >> V; cout << endl;
	cout << " количество граф: "; cin >> E; cout << endl;

	Graph<string> p(V);
	Graph<string> g(20) ;
	p.showAdj();
	for (int i = 0; i < V; ++i)
	{
		cout << " вершина: "; cin >> ver;
		p.insertVertex(ver);
		g.insertVertex(ver);
		cout << endl;
	}

	for (int i = 0; i < E; ++i)
	{
		cout << " Исходная: "; cin >> source; cout << endl;
		cout << " конечная: "; cin >> target; cout << endl;
		cout << " Вес: "; cin >> ves;

		string* targetVertPtr = &target;
		p.InsertEdge(source, target, ves);
		g.InsertEdge(source, target, ves);
	}
	
	cout << endl;
	g.Print();
	g.ADD("12",(V+1));
	g.InsertEdge("1", "12", 25);
	g.Draw();
	g.Print();
	vector<bool> visitedVerts(g.maxSize, false);	
}
