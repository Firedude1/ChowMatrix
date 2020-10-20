#include "NodeDetailsComponent.h"

namespace
{
    constexpr int xOffset = 5;
    constexpr int xPad = 10;
}

void doForNodes (DBaseNode* root, std::function<void(DelayNode*)> nodeFunc)
{
    for (int i = 0; i < root->getNumChildren(); ++i)
    {
        auto child = root->getChild (i);
        nodeFunc (child);
        doForNodes (child, nodeFunc);
    }
}

NodeDetailsComponent::NodeDetailsComponent (ChowMatrix& plugin) :
    plugin (plugin)
{
    setSize (1500, 200);

    for (auto& node : plugin.inputNodes)
    {
        node.addNodeListener (this);
        doForNodes (&node, [=] (DelayNode* node) { addNode (node); });
    }
}

NodeDetailsComponent::~NodeDetailsComponent()
{
    for (auto& node : plugin.inputNodes)
    {
        doForNodes (&node, [=] (DelayNode* node) { node->removeNodeListener (this); });
        node.removeNodeListener (this);
    }
}

void NodeDetailsComponent::addNode (DelayNode* node)
{
    addAndMakeVisible (nodes.add (std::make_unique<NodeInfo> (*node, false)));
    node->addNodeListener (this);
}

void NodeDetailsComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void NodeDetailsComponent::resized()
{
    for (int i = 0; i < nodes.size(); ++i)
    {
        nodes[i]->setBounds (xOffset + (NodeInfoConsts::InfoWidthNoLabel + xPad) * i, 0,
            NodeInfoConsts::InfoWidthNoLabel, getHeight());
    }
}

void NodeDetailsComponent::nodeAdded (DelayNode* newNode)
{
    addNode (newNode);

    MessageManagerLock mml;
    setSize (calcWidth(), getHeight());
    resized();
    repaint();
}

void NodeDetailsComponent::nodeRemoved (DelayNode* nodeToRemove)
{
    for (auto* node : nodes)
    {
        if (node->getNode() == nodeToRemove)
        {
            nodes.removeObject (node);
            break;
        }
    }

    nodeToRemove->removeNodeListener (this);

    MessageManagerLock mml;
    setSize (calcWidth(), getHeight());
    resized();
    repaint();
}

void NodeDetailsComponent::setMinWidth (int newMinWidth)
{
    minWidth = newMinWidth;
    setSize (calcWidth(), getHeight());
}

int NodeDetailsComponent::calcWidth()
{
    int width = 2 * xOffset + (xPad + NodeInfoConsts::InfoWidthNoLabel) * nodes.size();
    return jmax (width, minWidth);
}