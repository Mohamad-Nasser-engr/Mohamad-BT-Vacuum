#include <iostream>
#include <chrono>
#include "behaviortree_cpp_v3/action_node.h"
#include "behaviortree_cpp_v3/bt_factory.h"

using namespace std::chrono_literals;

/*Battery charge subtree*/
BT::NodeStatus batteryLow(BT::TreeNode &self){
    BT::Optional<int> msg = self.getInput<int>("battery_level");

    if  (!msg){
        
        throw BT::RuntimeError("missing required input message: ", msg.error());
    }
    
    std::cout<<"battery level is " <<msg.value()<< "%" <<  std::endl;
    if (msg.value()<=20){
        std::cout << "Robot has bad amount of battery" << std::endl;
        return BT::NodeStatus::FAILURE;
    }
    std::cout << "Robot has good amount of battery" << std::endl;
    return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus isAtDock(BT::TreeNode &self){
    BT::Optional<bool> msg = self.getInput<bool>("at_dock");

    if  (!msg){        
        throw BT::RuntimeError("missing required input message: ", msg.error());
    }

    if(msg.value()){
        std::cout << "Robot is at dock" << std::endl;
    return BT::NodeStatus::SUCCESS;
    }
     std::cout << "Robot is not at dock" << std::endl;
    return BT::NodeStatus::FAILURE  ;
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
    explicit Charge(const std::string &name, const BT::NodeConfiguration &config) : BT::SyncActionNode(name, config)
    {        
    }   

    // port
    static BT::PortsList providedPorts()
    {
        return {BT::OutputPort<int>("battery_level")};
    }
    //

    BT::NodeStatus tick() override
    {
        std::this_thread::sleep_for(3s);
        //edit battery lvel
        int battery = 100;
        BT::TreeNode::setOutput("battery_level", battery);
        //
        std::cout<<"Vacuum is fully charged"<<std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

/*Avoid obstacle subtree*/
BT::NodeStatus obstacle(BT::TreeNode &self){

    BT::Optional<bool> msg = self.getInput<bool>("at_obstacle");

    if  (!msg){        
        throw BT::RuntimeError("missing required input message: ", msg.error());
    }

    if(msg.value()){
         std::cout << "Found obstacle in front of robot" << std::endl;
        return BT::NodeStatus::FAILURE;

    }
    std::cout << "Did not found obstacle in front of robot" << std::endl;
    return BT::NodeStatus::SUCCESS;

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
    explicit Vacuum(const std::string &name, const BT::NodeConfiguration &config) : BT::SyncActionNode(name, config)
    {        
    }  

    // port
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<int>("battery_level"), BT::OutputPort<int>("battery_level")};
    }
    //

    BT::NodeStatus tick() override
    {
        BT::Optional<int> msg = getInput<int>("battery_level");
        if  (!msg){
        throw BT::RuntimeError("missing required input message: ", msg.error());
        }
        
        std::this_thread::sleep_for(3s);
        int battery = msg.value() -30;
        BT::TreeNode::setOutput("battery_level", battery);
        std::cout<<"Robot is vacuuming"<<std::endl;
        std::cout<<"Battery decreased to " << battery   << "%"<<std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

int main()
{
    BT::BehaviorTreeFactory factory;

    BT::PortsList battery_port = {BT::InputPort<int>("battery_level")};
    factory.registerSimpleCondition("BatteryLow", batteryLow, battery_port);

    BT::PortsList dock_port = {BT::InputPort<bool>("at_dock")};
    factory.registerSimpleCondition("IsAtDock", isAtDock, dock_port);

    factory.registerNodeType<GoToDock>("GoToDock");
    factory.registerNodeType<Charge>("Charge");

    BT::PortsList obstacle_port = {BT::InputPort<bool>("at_obstacle")};
    factory.registerSimpleCondition("Obstacle", obstacle, obstacle_port);
    factory.registerNodeType<AvoidObstacle>("AvoidObstacle");

    factory.registerNodeType<MoveForward>("MoveForward");

    factory.registerNodeType<Vacuum>("Vacuum");

    //create Tree
    auto tree = factory.createTreeFromFile("./../bt_vacuum.xml");

    //initialize blackboard variables
    tree.rootBlackboard()->set<int>("battery_level", 10);
    tree.rootBlackboard()->set<bool>("at_dock", true);
    tree.rootBlackboard()->set<bool>("at_obstacle", true);

    //execute the tree
    tree.tickRoot();
    
    

    return 0;
}