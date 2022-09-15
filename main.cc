#include <bits/stdc++.h>

using namespace std;

// 0: blanco, 1: gris, 2:negro
/*
ORDEN DE HIJOS
00: noroeste
01: noreste
10: suroeste
11: sureste
*/
enum class region_quad_color
{
    blanco = 0,
    gris,
    negro
};

struct region_quad_nodo
{
    region_quad_nodo *padre = nullptr;
    region_quad_color color = region_quad_color::blanco;
    array<array<region_quad_nodo*, 2>, 2> hijos{{{nullptr, nullptr}, {nullptr, nullptr}}};
};

region_quad_nodo* clonar(region_quad_nodo* obj, region_quad_nodo* obj_padre = nullptr){
    region_quad_nodo* ret = new region_quad_nodo{obj_padre, obj->color};
    if(obj->hijos[0][0]){
        ret->hijos[0][0] = clonar(obj->hijos[0][0], ret);
        ret->hijos[0][1] = clonar(obj->hijos[0][1], ret);
        ret->hijos[1][0] = clonar(obj->hijos[1][0], ret);
        ret->hijos[1][1] = clonar(obj->hijos[1][1], ret);
    }
    return ret;
}

class region_quad_tree
{
public:
    region_quad_tree(const region_quad_tree& rqt){
        x = rqt.x;
        y = rqt.y;
        raiz = clonar(rqt.raiz);
    }
    region_quad_tree(int x_, int y_) : x(x_), y(y_), raiz(new region_quad_nodo) {}
    void insertar(int _x, int _y)
    {
        insertar_recursiva(raiz, _x, _y, 1, x, 1, y);
    }
    void print()
    {
        print_recursiva(raiz);
    }
    friend region_quad_tree interceptar(region_quad_tree rq1, region_quad_tree rq2);
    friend region_quad_tree unir(region_quad_tree rq1, region_quad_tree rq2);

protected:
    void unir_rec(region_quad_nodo *&nodo1, region_quad_nodo *nodo2)
    {
        if(!nodo1) return;
        else if(nodo1->color == region_quad_color::negro) return;
        else if(nodo2->color == region_quad_color::blanco) return;
        else if(nodo1->color == region_quad_color::blanco && nodo2->color == region_quad_color::negro){
            nodo1->color = region_quad_color::negro;
            subida_colapsar(nodo1->padre);
        }
        else if(nodo1->color == region_quad_color::gris && nodo2->color == region_quad_color::negro){
            nodo1->color = region_quad_color::negro;
            nodo1->hijos[0][0] = nullptr;
            nodo1->hijos[0][1] = nullptr;
            nodo1->hijos[1][0] = nullptr;
            nodo1->hijos[1][1] = nullptr;
            subida_colapsar(nodo1->padre);
        }
        else if(nodo1->color == region_quad_color::blanco && nodo2->color == region_quad_color::gris){
            nodo1->color = nodo2->color;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    nodo1->hijos[i][j] = new region_quad_nodo{nodo1, region_quad_color::blanco};
                    unir_rec(nodo1->hijos[i][j], nodo2->hijos[i][j]);
                }
            }
        }
        else{
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    unir_rec(nodo1->hijos[i][j], nodo2->hijos[i][j]);
                }
            }
        }
    }
    void interceptar_rec(region_quad_nodo *&nodo1, region_quad_nodo *nodo2)
    {
        if(!nodo1) return;
        if (nodo1->color == region_quad_color::blanco)
            return;
        else if (nodo1->color == region_quad_color::negro && nodo1->color == nodo2->color)
            return;
        else if (nodo1->color == region_quad_color::gris && nodo2->color == region_quad_color::negro)
            return;
        else if (nodo1->color == region_quad_color::negro && nodo2->color == region_quad_color::gris){
            nodo1->color = nodo2->color;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    nodo1->hijos[i][j] = new region_quad_nodo{nodo1, region_quad_color::negro};
                    interceptar_rec(nodo1->hijos[i][j], nodo2->hijos[i][j]);
                }
            }
        }
        else if (nodo2->color == region_quad_color::blanco)
        {
            nodo1->color = region_quad_color::blanco;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    delete nodo1->hijos[i][j];
                    nodo1->hijos[i][j] = nullptr;
                }
            }
            subida_colapsar(nodo1->padre);
        }
        else{
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    interceptar_rec(nodo1->hijos[i][j], nodo2->hijos[i][j]);
                }
            }
        }
    }

    void print_recursiva(region_quad_nodo *nodo, int tab_n = 0)
    {
        string color_name;
        if (nodo->color == region_quad_color::blanco)
            color_name = "blanco";
        else if (nodo->color == region_quad_color::gris)
            color_name = "gris";
        else
            color_name = "negro";
        cout << color_name << '\n';
        if (nodo->hijos[0][0] == nullptr)
            return;
        for (int i = 0; i < tab_n; i++)
            cout << '\t';
        cout << "[" << endl;
        for (int i = 0; i < tab_n; i++)
            cout << '\t';
        print_recursiva(nodo->hijos[0][0], tab_n + 1);
        for (int i = 0; i < tab_n; i++)
            cout << '\t';
        print_recursiva(nodo->hijos[0][1], tab_n + 1);
        for (int i = 0; i < tab_n; i++)
            cout << '\t';
        print_recursiva(nodo->hijos[1][0], tab_n + 1);
        for (int i = 0; i < tab_n; i++)
            cout << '\t';
        print_recursiva(nodo->hijos[1][1], tab_n + 1);
        cout << endl;
        for (int i = 0; i < tab_n; i++)
            cout << '\t';
        cout << "]" << endl;
    }
    
    void insertar_recursiva(region_quad_nodo *&nodo, int _x, int _y, int x1, int x2, int y1, int y2)
    {
        if (nodo->color == region_quad_color::blanco)
        {
            int x_mid = (x1 + x2) / 2;
            int y_mid = (y1 + y2) / 2;
            if (x1 == x2)
            {
                nodo->color = region_quad_color::negro;
                subida_colapsar(nodo->padre);
            }
            else
            {
                nodo->color = region_quad_color::gris;
                nodo->hijos[0][0] = new region_quad_nodo{nodo};
                nodo->hijos[1][0] = new region_quad_nodo{nodo};
                nodo->hijos[1][1] = new region_quad_nodo{nodo};
                nodo->hijos[0][1] = new region_quad_nodo{nodo};
                insertar_recursiva(nodo->hijos[y_mid >= _y][!(_x <= x_mid)], _x, _y, (_x <= x_mid ? x1 : x_mid + 1), (_x > x_mid ? x2 : x_mid), (_y <= y_mid ? y1 : y_mid + 1), (_y > y_mid ? y2 : y_mid));
            }
        }
        else if (nodo->color == region_quad_color::gris)
        {
            int x_mid = (x1 + x2) / 2;
            int y_mid = (y1 + y2) / 2;
            insertar_recursiva(nodo->hijos[y_mid >= _y][!(_x <= x_mid)], _x, _y, (_x <= x_mid ? x1 : x_mid + 1), (_x > x_mid ? x2 : x_mid), (_y <= y_mid ? y1 : y_mid + 1), (_y > y_mid ? y2 : y_mid));
        }
    }
    void subida_colapsar(region_quad_nodo *&nodo)
    {
        if (nodo == nullptr)
            return;
        if (nodo->color == region_quad_color::gris)
        {
            int negro{};
            int blanco{};
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    if (nodo->hijos[i][j]->color == region_quad_color::negro)
                        negro++;
                    else if (nodo->hijos[i][j]->color == region_quad_color::blanco)
                        blanco++;                    
                }
            }
            if(negro == 4 || blanco == 4){
                nodo->color = (negro == 4?region_quad_color::negro : region_quad_color::blanco);
                nodo->hijos[0][0] = nullptr;
                nodo->hijos[0][1] = nullptr;
                nodo->hijos[1][0] = nullptr;
                nodo->hijos[1][1] = nullptr;
                subida_colapsar(nodo->padre);
            }
        }
    }

private:
    region_quad_nodo *raiz;
    int x, y;
};

region_quad_tree unir(region_quad_tree rq1, region_quad_tree rq2){
    region_quad_tree rqunir(rq1);
    rqunir.unir_rec(rqunir.raiz, rq2.raiz);
    return rqunir;
}
region_quad_tree interceptar(region_quad_tree rq1, region_quad_tree rq2)
{
    region_quad_tree rqinterceptar(rq1);
    rqinterceptar.interceptar_rec(rqinterceptar.raiz, rq2.raiz);
    return rqinterceptar;
}


int main()
{

    region_quad_tree arbolito_cruz(8, 8), arbolito_aspa(8,8);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        arbolito_cruz.insertar(x + 1, y + 1);
    }

    cin>>n;
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        arbolito_aspa.insertar(x + 1, y + 1);
    }

    region_quad_tree arbolito_int = interceptar(arbolito_cruz, arbolito_aspa);
    region_quad_tree arbolito_un = unir(arbolito_cruz, arbolito_aspa);

    cout<<"CRUZ:\n";
    arbolito_cruz.print();
    cout<<"\nASPA:\n";
    arbolito_aspa.print();
    cout<<"\nINTERCEPTAR:\n";
    arbolito_int.print();
    cout<<"\nUNIR:\n";
    arbolito_un.print();

    return 0;
}