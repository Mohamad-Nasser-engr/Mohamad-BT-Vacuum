#include <iostream>
#include <chrono>
#include "behaviortree_cpp_v3/action_node.h"
#include "behaviortree_cpp_v3/bt_factory.h"

using namespace std::chrono_literals;

/*Battery charge subtree*/
BT::NodeStatus batteryLow(){
    std::cout << "Robot has good amount oof battery" << std::endl;
    return BT::NodeStatus::FAILURE;
}

BT::NodeStatus isAtDock(){
     std::cout << "Robot is not at dock" << std::endl;
    return BT::NodeStatus::FAILURE;
}

class GoToDock : public BT::SyncActionNode
{
public:
    explicit GoToDock(const std::string &name) : BT::SyncActionNode(name, {})
    {        
    }    
    BT::NodeStatus tick() override
    {
        std::this_thread::sleep_for(3s);
        std::cout<<"Went to Dock"<<std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

class Charge : public BT::SyncActionNode
{
public:
    explicit Charge(const std::string &name) : BT::SyncActionNode(name, {})
    {        
    }    
    BT::NodeStatus tick() override
    {
        std::this_thread::sleep_for(3s);
        std::cout<<"Vacuum is fully charged"<<std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

/*Avoid obstacle subtree*/
BT::NodeStatus obstacle(){
     std::cout << "No obstacle in front of robot" << std::endl;
    return BT::NodeStatus::FAILURE;
}

class AvoidObstacle : public BT::SyncActionNode
{
public:
    explicit AvoidObstacle(const std::string &name) : BT::SyncActionNode(name, {})
    {        
    }    
    BT::NodeStatus tick() override
    {
        std::this_thread::sleep_for(3s);
        std::cout<<"Obstacle Avoided"<<std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};
/*Move forward*/
class MoveForward : public BT::SyncActionNode
{
public:
    explicit MoveForward(const std::string &name) : BT::SyncActionNode(name, {})
    {        
    }    
    BT::NodeStatus tick() override
    {
        std::this_thread::sleep_for(3s);
        std::cout<<"Vacuum is moving forward"<<std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

/*Vacuum*/
class Vacuum : public BT::SyncActionNode
{
public:
    explicit Vacuum(const std::string &name) : BT::SyncActionNode(name, {})
    {        
    }    
    BT::NodeStatus tick() override
    {
        std::this_thread::sleep_for(3s);
        std::cout<<"Robot is vacuuming"<<std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

int main()
{
    BT::BehaviorTreeFactory factory;
    factory.registerSimpleCondition("BatteryLow", std::bind(batteryLow));
    factory.registerSimpleCondition("IsAtDock", std::bind(isAtDock));
    factory.registerNodeType<GoToDock>("GoToDock");
    factory.registerNodeType<Charge>("Charge");

    factory.registerSimpleCondition("Obstacle", std::bind(obstacle));
    factory.registerNodeType<AvoidObstacle>("AvoidObstacle");

    factory.registerNodeType<MoveForward>("MoveForward");

    factory.registerNodeType<Vacuum>("Vacuum");

    //create Tree
    auto tree = factory.createTreeFromFile("./../bt_vacuum.xml");

    //execute the tree
    tree.tickRoot();
    
    return 0;
}