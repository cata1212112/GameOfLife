#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <random>
#include <vector>

using namespace sf;

const int d_lin[8] = {-1,-1,-1,0,0,1,1,1};
const int d_col[8] = {-1,0,1,-1,1,-1,0,1};

int x,y,z,simsizex,simsizey;
VertexArray celule(Quads);
VertexArray linii(Lines);
bool alive[5005][5905],acum[5005][5905];
int cell_size;
bool ok = 0;
bool start = 0;

int Get_Index(int i,int j)
{
    return (i*simsizex + j);
}

int Functie(int x,int y)
{
    return (x*simsizey + y)*4;
}

void Add_Cell(int i,int j)
{
    int index;
    int posx = i*z;
    int posz = j*z;
    Vertex vr1,vr2,vr3,vr4;
    Vector2f pos1(i,j),pos2(i+cell_size,j),pos3(i+cell_size,j+cell_size),pos4(i,j+cell_size);
    vr1.position = pos1;
    vr2.position = pos2;
    vr3.position = pos3;
    vr4.position = pos4;
    vr1.color = Color::Black;
    vr2.color = Color::Black;
    vr3.color = Color::Black;
    vr4.color = Color::Black;
    celule.append(vr1);
    celule.append(vr2);
    celule.append(vr3);
    celule.append(vr4);
}

void Update_Screen()
{
    int index;
    for (int i=0; i<simsizex; i++)
    {
        for (int j=0; j<simsizey; j++)
        {
            if (acum[i][j] == 1)
            {
                index = Functie(i,j);
                for (int k=0; k<4; k++)
                {
                    celule[index+k].color = Color::White;
                }
            }
            else
            {
                index = Functie(i,j);
                for (int k=0; k<4; k++)
                {
                    celule[index+k].color = Color::Black;
                }
            }
        }
    }
}

bool inside(int x,int y)
{
    return x>=0 && x<simsizex && y>=0 && y<simsizey;
}

int numar_vecini(int x,int y)
{
    int result=0;
    for(int i=-1; i<=1; i++)
    {
        for(int k=-1; k<=1; k++)
        {
            if((i!=0 || k!=0) &&  inside(x+i,y+k) && alive[x+i][y+k])
            {
                result++;
            }

        }
    }
    return result;
}


void Update_Game()
{
    for (int i=0; i<simsizex; i++)
    {
        for (int j=0; j<simsizey; j++)
        {

            acum[i][j] = alive[i][j];
            int nr = numar_vecini(i,j) ;
            ///std::cout << nr << " ";
            if (acum[i][j] == 1)
            {
                if (nr > 3 || nr < 2)
                {
                    acum[i][j] = 0;
                }
            }
            else
            {
                if (nr == 3)
                {
                    acum[i][j] = 1;
                }
            }
        }
    }
    Update_Screen();
    for (int i=0; i<simsizex; i++)
    {
        for (int j=0; j<simsizey; j++)
        {

            alive[i][j] = acum[i][j];
        }
    }
}

void Create_Map()
{
    for (int i=0; i<x; i += cell_size)
    {
        Vertex v1;
        v1.position = {0,i};
        v1.color = Color::White;
        Vertex v2;
        v2.position = {x,i};
        v2.color = Color::White;
        linii.append(v1);
        linii.append(v2);
    }
    for (int j=0; j<y; j += cell_size)
    {
      Vertex v1;
        v1.position = {j,0};
        v1.color = Color::White;
        Vertex v2;
        v2.position = {j,y};
        v2.color = Color::White;
        linii.append(v1);
        linii.append(v2);
    }
}

int main()
{
    std::cin >> x >> y >> z;
    cell_size = z;
    RenderWindow window(VideoMode(x, y), "SFML works!");
    window.setFramerateLimit(60);
    simsizex = x/z;
    simsizey = y/z;
    Create_Map();
    int ok = 0;
    for (int i=0; i<x; i+=cell_size)
    {
        for (int j=0; j<y; j+=cell_size)
        {
            Add_Cell(i,j);
        }
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (Mouse::isButtonPressed(Mouse::Left) && ok == 0)
        {
            ok = 1;
        }
        if (ok == 1)
        {
            Vector2i vec = Mouse::getPosition(window);
            vec.x = ceil(vec.x/cell_size);
            vec.y = ceil(vec.y/cell_size);
            alive[vec.x][vec.y] = 1;
            int index = Functie(vec.x,vec.y);
            for (int k=0; k<4; k++)
            {
                celule[index+k].color = Color::White;
            }
        }
        if (Mouse::isButtonPressed(Mouse::Left) && ok == 1)
        {
            ok = 0;
        }

        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            start = 1;
        }
        if (start == 1)
        {
            Update_Game();
        }
        window.clear();
        window.draw(celule);
        window.draw(linii);
        window.display();
    }
    return 0;
}
