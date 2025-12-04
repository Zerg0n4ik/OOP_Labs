#ifndef LAB6_HPP
#define LAB6_HPP
#include <vector>
#include <fstream>
#include <memory>
#include <cmath>
#include <string>

class Observer {
public:
    virtual void onKill(const std::string& killer, const std::string& victim) = 0;
    virtual ~Observer() {}
};

class ScreenObserver : public Observer {
public:
    void onKill(const std::string& killer, const std::string& victim) override;
};

class FileObserver : public Observer {
    std::ofstream log;
public:
    FileObserver();
    void onKill(const std::string& killer, const std::string& victim) override;
};

class Visitor;

class NPC {
protected:
    std::string name;
    int x{}, y{};
public:
    NPC(std::string n, int X, int Y);
    virtual ~NPC() {}

    virtual std::string getType() const = 0;
    virtual bool canKill(const NPC& other) const = 0;

    int getX() const { return x; }
    int getY() const { return y; }
    std::string getName() const { return name; }

    double dist(const NPC& other) const;
    virtual void accept(Visitor& v, NPC& other) = 0;
};

class Dragon : public NPC {
public:
    using NPC::NPC;
    std::string getType() const override;
    bool canKill(const NPC& other) const override;
    void accept(Visitor& v, NPC& other) override;
};

class Bull : public NPC {
public:
    using NPC::NPC;
    std::string getType() const override;
    bool canKill(const NPC& other) const override;
    void accept(Visitor& v, NPC& other) override;
};

class Frog : public NPC {
public:
    using NPC::NPC;
    std::string getType() const override;
    bool canKill(const NPC& other) const override;
    void accept(Visitor& v, NPC& other) override;
};

class Visitor {
public:
    virtual bool fight(NPC& a, NPC& b) = 0;
};

class FightVisitor : public Visitor {
    std::vector<Observer*>& observers;
public:
    FightVisitor(std::vector<Observer*>& obs);
    bool fight(NPC& a, NPC& b) override;
};

class NPCFactory {
public:
    static std::unique_ptr<NPC> create(const std::string& type,
                                       const std::string& name,
                                       int x, int y);
};

class Editor {
    std::vector<std::unique_ptr<NPC>> npcs;
    std::vector<Observer*> observers;
public:
    void addNPC(const std::string& type, const std::string& name, int x, int y);
    void attachObserver(Observer* o);

    void printAll() const;
    void save(const std::string& file);
    void load(const std::string& file);

    void battle(double radius);
};

#endif