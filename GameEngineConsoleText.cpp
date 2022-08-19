// GameEngineConsoleText.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

namespace ECS {

    
    struct Position {
        uint8_t x, y;
    };

    struct Component_t {
        
    };
 
    class Entity_t {
    
    public:

        template<typename T>
        std::size_t getComponentByID() {
            static std::size_t type{ ++nextID };
            return type;
        }

        template<typename T_CMP>
        void addComponent(T_CMP& component) {

            auto type = getComponentByID<T_CMP>();
            mapComponents[type] = &component;
        }


        template<typename T_CMP>
        T_CMP* getComponent() {
            auto type = getComponentByID<T_CMP>();
            auto it = mapComponents.find(type);
            if (it != mapComponents.end()) {

                return static_cast<T_CMP*>(it->second);
            }
            return nullptr;
        }

        template<typename T_CMP>
        const T_CMP* getComponent() const {
            auto p = const_cast<Entity_t*>(this)->getComponent<T_CMP>();
            return const_cast<const T_CMP*>(p);
        }
    
    private:
        inline static std::size_t nextID{ 0 };
        std::unordered_map<std::size_t, Component_t*> mapComponents;

    };

    

  


}//namespace ECS 


struct PositionComponent_t : ECS::Component_t {

    //PositionComponent_t(ECS::Position position) : pos(position) {};
    ECS::Position pos;
};

template<typename T>
struct RenderComponent_t : ECS::Component_t {
    T w, h;
    char sp;
};


void locate(ECS::Position p) {
    std::printf("\033[%d;%dH", p.x, p.y);
}


void completePutchar(uint8_t w, uint8_t h, char sp, ECS::Position p) {

    auto auxP = p;
    for (std::size_t i = 0; i < h; i++) {
        for (std::size_t j = 0; j < w; j++) {
            std::putchar(sp);
        }
        p.x++;
        locate(p);
    }


}

template<typename TYPE,typename ENT>
void draw(const ENT& e) {

    auto* p = (e.getComponent<PositionComponent_t>()); 
    auto* r = (e.getComponent<TYPE>());  //Es neceasrio el e.template 

    if (p && r) {

        locate(p->pos);
        completePutchar(r->w, r->h, r->sp,p->pos);
    }
}  

void clearScreen(){ std::puts("\033[H\033[J"); }
int main()
{

    std::vector<ECS::Entity_t> entities;
    entities.reserve(10);
    
    auto& player1 = entities.emplace_back();
    
    ECS::Position positionPlayer1{ 5,6 };

    RenderComponent_t<uint8_t> renderPlayer1{ {},4,4,'X' };
    PositionComponent_t posComponent1{ {},positionPlayer1 };
    
    //Adding Components
    player1.addComponent(posComponent1);
    player1.addComponent(renderPlayer1);

    clearScreen();
    
    std::for_each(entities.begin(), entities.end(), draw<RenderComponent_t<uint8_t>,ECS::Entity_t>);
   
    std::getchar();

    return 0;
}


