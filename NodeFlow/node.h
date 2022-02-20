#ifndef NODE_H
#define NODE_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <memory>
#include <map>
#include <wx/gdicmn.h>
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>

namespace NODEFLOW
{

typedef std::vector<unsigned> ItemList; // list of indexed items
typedef std::unique_ptr<ItemList> ItemListPtr;

#define CONNECTION_SIZE (10)
#define CONNECTION_SPACE (3 * CONNECTION_SIZE)
#define NODE_RECT_WIDTH (50)
#define NODE_RECT_HEIGHT_MARGIN (10)
#define NODE_TITLE_BAR_HEIGHT (20)
//
#define DATA_PAYLOAD "payload"
#define DATA_TOPIC "topic"
#define DATA_MSGID "msgid"
#define VALUE_TAG "Value"
#define STATE_TAG "State"


//
class NodeSet;
class NodeLayout;
class NodeType;

typedef Json::Value VALUE;
typedef std::map<unsigned,VALUE> VALUEMAP;

class Node
{
    //
    unsigned _id = 0;
    unsigned  _type = 0;
    std::string _sid; // id string
    wxPoint _location; // location of the node
    //
    std::vector<ItemListPtr> _inputs; // the inputs to the node
    std::vector<ItemListPtr> _outputs; // the outputs from the node
    bool _selected = false;
    wxColour _colour = wxColour(0xFFFFFFFF); // node colour
    VALUE _data; // data for processing - runtime volatile
    VALUEMAP _calculatedData; // cache calculated data
    bool _enabled = true; // is the node active
    bool _calculated = false; // has the output been calculated

public:
    //
    Node(unsigned id = 0, size_t type = 0);
    Node(const Node &) = delete;
    virtual ~Node() {}
    //
    wxPoint location() const { return _location;}
    void setLocation(wxPoint pt) { _location = pt;}
    void setLocation(int x, int y) {_location.x = x; _location.y = y;}
    const NodeLayout &layout();
    NodeType * nodeType() const;
    //
    VALUE & data() { return _data;}
    VALUEMAP & calculatedData() { return _calculatedData;}
    //
    unsigned id() const {
        return _id;
    }
    void setId(unsigned id) {
        _id = id;
        _sid = std::to_string(id);
    }
    //
    unsigned type() const {
        return _type;   // the node type
    }
    void setType(size_t t) {
        _type = t;
    }

    wxColour colour() const { return _colour;}
    void setColour(const wxColour &c) { _colour = c;}
    //
    const std::string & sid() const {
        return  _sid;
    }
    /*!
     * \brief selected
     * \return
     */
    bool selected() const
    {
        return _selected;
    }
    /*!
     * \brief setSelected
     * \param f
     */
    void setSelected(bool f = true)
    {
        _selected = f;
    }
    /*!
     * \brief enabled
     * \return
     */
    bool enabled() const { return _enabled ;}
    /*!
     * \brief setEnabled
     * \param f
     */
    void setEnabled(bool f = true) { _enabled = f;}
    //
    bool calculated() const { return _calculated;}
    //
    void setCalculated(bool f = true) { _calculated = f;}
    //
    std::vector<ItemListPtr> &inputs()  { return _inputs;}  // the inputs to the node
    std::vector<ItemListPtr> &outputs() { return _outputs;} // the outputs from the node

    void setInputs(size_t n) {
        _inputs.resize(n);
        for(size_t i = 0; i < n; i++)
        {
            _inputs[i] = std::make_unique<ItemList>();
        }
    }
    void setOutputs(size_t n) {
        _outputs.resize(n);
        for(size_t i = 0; i < n; i++)
        {
            _outputs[i] = std::make_unique<ItemList>();
        }
    }

    /*!
     * \brief toPath
     * \param p
     */
    void toPath(MRL::PropertyPath &p) // path in tree to a node
    {
        p.clear();
        p.push_back("Nodes");
        p.push_back(sid());
    }
    /*!
     * \brief load
     * \param s
     */
    virtual void load(NodeSet &s);
    /*!
     * \brief save
     * \param s
     */
    virtual void save(NodeSet &s);

};

typedef std::unique_ptr<Node> NodePtr;
typedef std::map<unsigned,NodePtr> NodePtrMap;

}
#endif // NODE_H
