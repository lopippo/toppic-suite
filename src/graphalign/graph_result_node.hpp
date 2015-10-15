#ifndef PROT_GRAPH_RESULT_NODE_HPP_
#define PROT_GRAPH_RESULT_NODE_HPP_

#include <memory>
#include <vector>

namespace prot {

class GraphResultNode { 
 public:
  GraphResultNode(GraphDpNodePtr node_ptr, int s, int m) {
    first_idx_ = node_ptr->getFirstIdx();
    second_idx_ = node_ptr->getSecondIdx();
    shift_num_ = s;
    mod_num_ = m;
    prev_edge_type_ = node_ptr->getPrevEdgeType(s, m);
  }

  int getFirstIdx() {return first_idx_;}
  int getSecondIdx() {return second_idx_;}
  int getShiftNum(){return shift_num_;}
  int getModNum(){return mod_num_;}
  int getPrevEdgeType() {return prev_edge_type_;}

 private:
  int first_idx_;
  int second_idx_;
  int shift_num_;
  int mod_num_;
  int prev_edge_type_;
};

typedef std::shared_ptr<GraphResultNode> GraphResultNodePtr;
typedef std::vector<GraphResultNodePtr> GraphResultNodePtrVec;
typedef std::vector<GraphResultNodePtrVec> GraphResultNodePtrVec2D;
typedef std::vector<GraphResultNodePtrVec2D> GraphResultNodePtrVec3D;

} /* namespace prot */

#endif /* GRAPH_RESULT_NODE_HPP_ */
