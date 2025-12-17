#include "../include/lab7.hpp"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <ctime>

std::mutex global_cout_mutex;

void ScreenObserver::onKill(const std::string& killer, const std::string& victim) {
    std::lock_guard<std::mutex> lock(global_cout_mutex);
    std::cout << killer << " убивает " << victim << "\n";
}

FileObserver::FileObserver() {
    log.open("log.txt", std::ios::app);
}

void FileObserver::onKill(const std::string& killer, const std::string& victim) {
    std::lock_guard<std::mutex> lock(log_mutex);
    log << killer << " убивает " << victim << "\n";
    log.flush();
}

NPC::NPC(std::string n, int X, int Y) : name(std::move(n)), x(X), y(Y) {}

double NPC::dist(const NPC& other) const {
    return std::hypot(x - other.x, y - other.y);
}

void NPC::moveRandom() {
    if (!alive) return;
    
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::uniform_int_distribution<> dir_dist(-1, 1);
    
    int newX = x + dir_dist(gen);
    int newY = y + dir_dist(gen);
    
    newX = std::max(0, std::min(MAP_WIDTH - 1, newX));
    newY = std::max(0, std::min(MAP_HEIGHT - 1, newY));
    
    x = newX;
    y = newY;
}

bool NPC::moveTowards(int targetX, int targetY, int distance) {
    if (!alive) return false;
    
    int dx = targetX - x;
    int dy = targetY - y;
    double dist = std::hypot(dx, dy);
    
    if (dist <= distance) {
        x = targetX;
        y = targetY;
        return true;
    }
    
    double ratio = distance / dist;
    x += static_cast<int>(dx * ratio);
    y += static_cast<int>(dy * ratio);
    
    x = std::max(0, std::min(MAP_WIDTH - 1, x));
    y = std::max(0, std::min(MAP_HEIGHT - 1, y));
    
    return false;
}

std::string Dragon::getType() const { return "Дракон"; }
int Dragon::getMoveDistance() const { return 50; }
int Dragon::getKillDistance() const { return 30; }
bool Dragon::canKill(const NPC& other) const {
    return other.getType() == "Бык";
}
void Dragon::accept(Visitor& v, NPC& other) { v.fight(*this, other); }

std::string Bull::getType() const { return "Бык"; }
int Bull::getMoveDistance() const { return 30; }
int Bull::getKillDistance() const { return 10; }
bool Bull::canKill(const NPC& other) const {
    return other.getType() == "Жаба";
}
void Bull::accept(Visitor& v, NPC& other) { v.fight(*this, other); }

std::string Frog::getType() const { return "Жаба"; }
int Frog::getMoveDistance() const { return 1; }
int Frog::getKillDistance() const { return 10; }
bool Frog::canKill(const NPC& other) const {
    (void)other;
    return false;
}
void Frog::accept(Visitor& v, NPC& other) { v.fight(*this, other); }

FightVisitor::FightVisitor(std::vector<Observer*>& obs, std::mutex& mtx) 
    : observers(obs), cout_mutex(mtx) {}

bool FightVisitor::fight(NPC& a, NPC& b) {
    if (!a.isAlive() || !b.isAlive()) return false;
    
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::uniform_int_distribution<> dice(1, 6);
    
    int attack_power = dice(gen);
    int defense_power = dice(gen);
    
    bool someone_died = false;
    
    if (a.canKill(b) && attack_power > defense_power) {
        b.kill();
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            for (auto* o : observers) {
                o->onKill(a.getName(), b.getName());
            }
        }
        someone_died = true;
    }
    
    if (b.canKill(a) && attack_power > defense_power) {
        a.kill();
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            for (auto* o : observers) {
                o->onKill(b.getName(), a.getName());
            }
        }
        someone_died = true;
    }
    
    return someone_died;
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

Editor::Editor() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

Editor::~Editor() {
    stop();
}

void Editor::addNPC(const std::string& type, const std::string& name, int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        throw std::invalid_argument("Координаты вне диапазона");
    }
    
    std::unique_lock<std::shared_mutex> lock(npcs_mutex);
    npcs.push_back(NPCFactory::create(type, name, x, y));
}

void Editor::attachObserver(Observer* o) {
    observers.push_back(o);
}

void Editor::generateRandomNPCs(int count) {
    std::vector<std::string> types = {"Дракон", "Бык", "Жаба"};
    
    for (int i = 0; i < count; ++i) {
        std::string type = types[std::rand() % types.size()];
        std::string name = type + "_" + std::to_string(i + 1);
        int x = std::rand() % MAP_WIDTH;
        int y = std::rand() % MAP_HEIGHT;
        
        addNPC(type, name, x, y);
    }
}

void Editor::printAll() const {
    std::shared_lock<std::shared_mutex> lock(npcs_mutex);
    
    std::lock_guard<std::mutex> cout_lock(cout_mutex);
    std::cout << "Все NPC (живые и мертвые):\n";
    for (auto& npc : npcs) {
        std::string status = npc->isAlive() ? "жив" : "мертв";
        std::cout << npc->getType() << " "
                  << npc->getName() << " ("
                  << npc->getX() << "," << npc->getY() << ") - "
                  << status << "\n";
    }
}

void Editor::movementWorker() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::shared_lock<std::shared_mutex> lock(npcs_mutex);
        
        for (auto& npc : npcs) {
            if (npc->isAlive()) {
                npc->moveRandom();
            }
        }
        
        for (size_t i = 0; i < npcs.size(); ++i) {
            if (!npcs[i]->isAlive()) continue;
            
            for (size_t j = i + 1; j < npcs.size(); ++j) {
                if (!npcs[j]->isAlive()) continue;
                
                double distance = npcs[i]->dist(*npcs[j]);
                int killDistance = std::max(npcs[i]->getKillDistance(), 
                                           npcs[j]->getKillDistance());
                
                if (distance <= killDistance) {
                    std::lock_guard<std::mutex> task_lock(task_mutex);
                    FightTask task;

                    if (std::rand() % 2 == 0) {
                        task.attacker = npcs[i].get();
                        task.defender = npcs[j].get();
                    } else {
                        task.attacker = npcs[j].get();
                        task.defender = npcs[i].get();
                    }
                    
                    fight_tasks.push(task);
                    task_cv.notify_one();
                }
            }
        }
    }
}

void Editor::fightWorker() {
    while (running) {
        std::unique_lock<std::mutex> lock(task_mutex);

        task_cv.wait_for(lock, std::chrono::milliseconds(100), 
            [this]() { return !fight_tasks.empty() || !running; });
        
        while (!fight_tasks.empty() && running) {
            FightTask task = fight_tasks.front();
            fight_tasks.pop();
            
            lock.unlock();

            if (task.attacker->isAlive() && task.defender->isAlive()) {
                FightVisitor visitor(observers, cout_mutex);
                visitor.fight(*task.attacker, *task.defender);
            }
            
            lock.lock();
        }
    }
}

void Editor::printWorker() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        {
            std::lock_guard<std::mutex> cout_lock(cout_mutex);
            std::cout << "\n=== Карта (живые NPC) ===\n";
            std::cout << "Легенда: D - Дракон, B - Бык, F - Жаба, . - пусто\n";

            std::vector<std::vector<char>> map(MAP_HEIGHT, std::vector<char>(MAP_WIDTH, '.'));

            {
                std::shared_lock<std::shared_mutex> lock(npcs_mutex);
                for (auto& npc : npcs) {
                    if (npc->isAlive()) {
                        int x = npc->getX();
                        int y = npc->getY();
                        
                        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                            char symbol = '.';
                            if (npc->getType() == "Дракон") symbol = 'D';
                            else if (npc->getType() == "Бык") symbol = 'B';
                            else if (npc->getType() == "Жаба") symbol = 'F';
                            
                            map[y][x] = symbol;
                        }
                    }
                }
                
                int alive_count = 0;
                int dragon_count = 0;
                int bull_count = 0;
                int frog_count = 0;
                
                for (auto& npc : npcs) {
                    if (npc->isAlive()) {
                        alive_count++;
                        if (npc->getType() == "Дракон") dragon_count++;
                        else if (npc->getType() == "Бык") bull_count++;
                        else if (npc->getType() == "Жаба") frog_count++;
                    }
                }
                
                std::cout << "Статистика:\n";
                std::cout << "  Всего живых NPC: " << alive_count << "\n";
                std::cout << "  Драконов: " << dragon_count << "\n";
                std::cout << "  Быков: " << bull_count << "\n";
                std::cout << "  Жаб: " << frog_count << "\n";
            }
            
            std::cout << "\nПолная карта (" << MAP_WIDTH << "x" << MAP_HEIGHT << "):\n";

            std::cout << "+";
            for (int x = 0; x < MAP_WIDTH; ++x) {
                std::cout << "-";
            }
            std::cout << "+\n";

            for (int y = 0; y < MAP_HEIGHT; ++y) {
                std::cout << "|";
                for (int x = 0; x < MAP_WIDTH; ++x) {
                    std::cout << map[y][x];
                }
                std::cout << "|\n";
            }
            
            std::cout << "+";
            for (int x = 0; x < MAP_WIDTH; ++x) {
                std::cout << "-";
            }
            std::cout << "+\n";
            
            std::cout << "=======================\n";
        }
    }
}

void Editor::start() {
    if (running) return;
    
    running = true;
    
    movement_thread = std::thread([this]() { this->movementWorker(); });
    fight_thread = std::thread([this]() { this->fightWorker(); });
    print_thread = std::thread([this]() { this->printWorker(); });
}

void Editor::stop() {
    if (!running) return;
    
    running = false;

    {
        std::lock_guard<std::mutex> lock(task_mutex);
        task_cv.notify_all();
    }

    if (movement_thread.joinable()) movement_thread.join();
    if (fight_thread.joinable()) fight_thread.join();
    if (print_thread.joinable()) print_thread.join();
}

void Editor::runGame(int seconds) {
    std::cout << "=== Начало игры на " << seconds << " секунд ===\n";
    
    start();
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    stop();
    
    std::cout << "\n=== Игра завершена ===\n";
    std::cout << "Выжившие NPC:\n";
    
    std::shared_lock<std::shared_mutex> lock(npcs_mutex);
    int survivor_count = 0;
    for (auto& npc : npcs) {
        if (npc->isAlive()) {
            std::cout << "- " << npc->getType() << " " << npc->getName() 
                      << " (" << npc->getX() << "," << npc->getY() << ")\n";
            survivor_count++;
        }
    }
    
    if (survivor_count == 0) {
        std::cout << "Нет выживших\n";
    }
}

void Editor::save(const std::string& file) {
    std::ofstream out(file);
    std::shared_lock<std::shared_mutex> lock(npcs_mutex);
    
    for (auto& npc : npcs) {
        if (npc->isAlive()) {
            out << npc->getType() << " " << npc->getName() << " "
                << npc->getX() << " " << npc->getY() << "\n";
        }
    }
}

void Editor::load(const std::string& file) {
    std::unique_lock<std::shared_mutex> lock(npcs_mutex);
    npcs.clear();
    
    std::ifstream in(file);
    std::string type, name;
    int x, y;

    while (in >> type >> name >> x >> y) {
        addNPC(type, name, x, y);
    }
}

void Editor::battle(double radius) {
    std::unique_lock<std::shared_mutex> lock(npcs_mutex);
    FightVisitor visitor(observers, cout_mutex);

    bool changed = true;
    int max_iterations = 100;
    int iteration = 0;
    
    while (changed && iteration < max_iterations) {
        changed = false;
        iteration++;

        std::vector<size_t> alive_indices;
        for (size_t i = 0; i < npcs.size(); ++i) {
            if (npcs[i]->isAlive()) {
                alive_indices.push_back(i);
            }
        }

        for (size_t idx_i = 0; idx_i < alive_indices.size(); ++idx_i) {
            size_t i = alive_indices[idx_i];
            if (!npcs[i]->isAlive()) continue;
            
            for (size_t idx_j = idx_i + 1; idx_j < alive_indices.size(); ++idx_j) {
                size_t j = alive_indices[idx_j];
                if (!npcs[j]->isAlive()) continue;
                
                if (npcs[i]->dist(*npcs[j]) <= radius) {
                    bool fight_result = visitor.fight(*npcs[i], *npcs[j]);
                    if (fight_result) {
                        changed = true;
                        break;
                    }
                }
            }
            if (changed) break;
        }

        for (auto it = npcs.begin(); it != npcs.end(); ) {
            if (!(*it)->isAlive()) {
                it = npcs.erase(it);
                changed = true;
            } else {
                ++it;
            }
        }
    }
    
    if (iteration >= max_iterations) {
        std::lock_guard<std::mutex> cout_lock(cout_mutex);
        std::cout << "Предупреждение: battle() достиг максимального количества итераций\n";
    }
}