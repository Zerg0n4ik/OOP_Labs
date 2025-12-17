#ifndef LAB7_HPP
#define LAB7_HPP

#include <vector>
#include <fstream>
#include <memory>
#include <cmath>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>

const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 100;

extern std::mutex global_cout_mutex;

class Observer {
public:
    virtual void onKill(const std::string& killer, const std::string& victim) = 0;
    virtual ~Observer() = default;
};

class ScreenObserver : public Observer {
public:
    void onKill(const std::string& killer, const std::string& victim) override;
};

class FileObserver : public Observer {
    std::ofstream log;
    std::mutex log_mutex;
public:
    FileObserver();
    void onKill(const std::string& killer, const std::string& victim) override;
};

class Visitor;

class NPC {
protected:
    std::string name;
    int x{}, y{};
    bool alive = true;
    
public:
    NPC(std::string n, int X, int Y);
    virtual ~NPC() = default;

    virtual std::string getType() const = 0;
    virtual int getMoveDistance() const = 0;
    virtual int getKillDistance() const = 0;
    virtual bool canKill(const NPC& other) const = 0;

    int getX() const { return x; }
    int getY() const { return y; }
    std::string getName() const { return name; }
    bool isAlive() const { return alive; }
    void kill() { alive = false; }

    double dist(const NPC& other) const;
    void moveRandom();
    bool moveTowards(int targetX, int targetY, int distance);
    
    virtual void accept(Visitor& v, NPC& other) = 0;
};

class Dragon : public NPC {
public:
    using NPC::NPC;
    std::string getType() const override;
    int getMoveDistance() const override;
    int getKillDistance() const override;
    bool canKill(const NPC& other) const override;
    void accept(Visitor& v, NPC& other) override;
};

class Bull : public NPC {
public:
    using NPC::NPC;
    std::string getType() const override;
    int getMoveDistance() const override;
    int getKillDistance() const override;
    bool canKill(const NPC& other) const override;
    void accept(Visitor& v, NPC& other) override;
};

class Frog : public NPC {
public:
    using NPC::NPC;
    std::string getType() const override;
    int getMoveDistance() const override;
    int getKillDistance() const override;
    bool canKill(const NPC& other) const override;
    void accept(Visitor& v, NPC& other) override;
};

class Visitor {
public:
    virtual bool fight(NPC& a, NPC& b) = 0;
    virtual ~Visitor() = default;
};

class FightVisitor : public Visitor {
    std::vector<Observer*>& observers;
    std::mutex& cout_mutex;
public:
    FightVisitor(std::vector<Observer*>& obs, std::mutex& mtx);
    bool fight(NPC& a, NPC& b) override;
};

class NPCFactory {
public:
    static std::unique_ptr<NPC> create(const std::string& type,
                                       const std::string& name,
                                       int x, int y);
};

struct FightTask {
    NPC* attacker;
    NPC* defender;
};

class Editor {
    std::vector<std::unique_ptr<NPC>> npcs;
    std::vector<Observer*> observers;
    
    mutable std::shared_mutex npcs_mutex;
    std::mutex task_mutex;
    mutable std::mutex cout_mutex;
    std::condition_variable task_cv;
    
    std::queue<FightTask> fight_tasks;
    std::atomic<bool> running{false};
    
    std::thread movement_thread;
    std::thread fight_thread;
    std::thread print_thread;
    
    void movementWorker();
    void fightWorker();
    void printWorker();
    
public:
    Editor();
    ~Editor();
    
    void addNPC(const std::string& type, const std::string& name, int x, int y);
    void attachObserver(Observer* o);
    void generateRandomNPCs(int count);
    
    void printAll() const;
    void save(const std::string& file);
    void load(const std::string& file);
    
    void start();
    void stop();
    void runGame(int seconds);
    
    void battle(double radius);
};

#endif