#ifndef TSP_NODE_H
#define TSP_NODE_H

class Node {
  public:
  /**
   * Node index
   * or City number
   */
  int index = 0;

  /**
   * x coordinate
   */
  int x = 0;

  /**
   * y coordinate
   */
  int y = 0;

  public:
    Node() = default;

    Node(int index, int x, int y);

    void printNode();
};

#endif //TSP_NODE_H