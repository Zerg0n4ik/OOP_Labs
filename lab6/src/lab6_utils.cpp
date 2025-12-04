#include "../include/lab6.hpp"
#include <iostream>

void ScreenObserver::onKill(const std::string& killer, const std::string& victim) {
    std::cout << killer << " убивает " << victim << "\n";
}

FileObserver::FileObserver() {
    log.open("log.txt", std::ios::app);
}

void FileObserver::onKill(const std::string& killer, const std::string& victim) {
    log << killer << " убивает " << victim << "\n";
    log.flush();
}

NPC::NPC(std::string n, int X, int Y) : name(std::move(n)), x(X), y(Y) {}

double NPC::dist(const NPC& other) const {
    return std::hypot(x - other.x, y - other.y);
}

std::string Dragon::getType() const { 
    return "Дракон"; 
}

bool Dragon::canKill(const NPC& other) const {
    return other.getType() == "Бык";
}

void Dragon::accept(Visitor& v, NPC& other) { 
    v.fight(*this, other); 
}

std::string Bull::getType() const { 
    return "Бык"; 
}

bool Bull::canKill(const NPC& other) const {
    return other.getType() == "Жаба";
}

void Bull::accept(Visitor& v, NPC& other) { 
    v.fight(*this, other); 
}

std::string Frog::getType() const { 
    return "Жаба"; 
}

bool Frog::canKill(const NPC& other) const {
    return false;
}

void Frog::accept(Visitor& v, NPC& other) { 
    v.fight(*this, other); 
}

FightVisitor::FightVisitor(std::vector<Observer*>& obs) : observers(obs) {}

bool FightVisitor::fight(NPC& a, NPC& b) {
    if (a.canKill(b)) {
        for (auto* o : observers) o->onKill(a.getName(), b.getName());
        return true;
    }
    if (b.canKill(a)) {
        for (auto* o : observers) o->onKill(b.getName(), a.getName());
        return false;
    }
    return false;
}

std::unique_ptr<NPC> NPCFactory::create(const std::string& type,
                                        const std::string& name,
                                        int x, int y)
{
    if (type == "Дракон") {
        return std::make_unique<Dragon>(name, x, y);
    }
    if (type == "Бык") {
        return std::make_unique<Bull>(name, x, y);
    }
    if (type == "Жаба") {
        return std::make_unique<Frog>(name, x, y);
    }

    throw std::runtime_error("Unknown NPC type");
}

void Editor::addNPC(const std::string& type, const std::string& name, int x, int y) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        throw std::invalid_argument("Координаты вне диапазона");
    }
    npcs.push_back(NPCFactory::create(type, name, x, y));
}

void Editor::attachObserver(Observer* o) {
    observers.push_back(o);
}

void Editor::printAll() const {
    for (auto& npc : npcs) {
        std::cout << npc->getType() << " "
                  << npc->getName() << " ("
                  << npc->getX() << "," << npc->getY() << ")\n";
    }
}

void Editor::save(const std::string& file) {
    std::ofstream out(file);
    for (auto& npc : npcs)
        out << npc->getType() << " " << npc->getName() << " "
            << npc->getX() << " " << npc->getY() << "\n";
}

void Editor::load(const std::string& file) {
    npcs.clear();
    std::ifstream in(file);
    std::string type, name;
    int x, y;

    while (in >> type >> name >> x >> y)
        addNPC(type, name, x, y);
}

void Editor::battle(double radius) {
    FightVisitor visitor(observers);

    bool changed = true;
    while (changed) {
        changed = false;
        bool restarted = false;

        for (size_t i = 0; i < npcs.size() && !restarted; ++i) {
            for (size_t j = i + 1; j < npcs.size(); ++j) {
                if (npcs[i]->dist(*npcs[j]) <= radius) {
                    bool iWins = npcs[i]->canKill(*npcs[j]);
                    bool jWins = npcs[j]->canKill(*npcs[i]);

                    if (!iWins && !jWins) continue;
                    if (iWins) {
                        for (auto* o : observers) o->onKill(npcs[i]->getName(), npcs[j]->getName());
                        npcs.erase(npcs.begin() + j);
                    } else {
                        for (auto* o : observers) o->onKill(npcs[j]->getName(), npcs[i]->getName());
                        npcs.erase(npcs.begin() + i);
                    }

                    changed = true;
                    restarted = true;
                    break;
                }
            }
        }
    }
}