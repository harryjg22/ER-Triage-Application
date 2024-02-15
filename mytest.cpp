// CMSC 341 - Fall 2023 - Project 3

#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

// Priority functions compute an integer priority for a patient.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);


class Tester{
public:

     void insertForTests(PQueue& heap){
          for (int i = 0; i < 300; i++){
               Patient patient(" ",MINTEMP,MINOX,MINRR + (i % MAXRR - MINRR),MINBP,MINOPINION);
               heap.insertPatient(patient);

          }
     }
    
     bool testMinHeapProperty(PQueue& myPQueue) {
          return testMinHeapRemovalOrder(myPQueue);
     }

     bool testMaxHeapProperty(PQueue& myPQueue) {
          return testMaxHeapRemovalOrder(myPQueue);
     }

     bool testMinHeapRemovalOrder(PQueue& myPQueue){
          while(myPQueue.numPatients() > 0){
               Patient prevPatient = myPQueue.m_heap->getPatient();
               int prevPatientPriority = myPQueue.m_priorFunc(prevPatient);
               Patient currPatient = myPQueue.getNextPatient();
               int currPatientPriority = myPQueue.m_priorFunc(currPatient);
               if(prevPatientPriority > currPatientPriority){
                    return false;
               }

          }
          return true;
     }

     bool testMaxHeapRemovalOrder(PQueue& myPQueue){
          while(myPQueue.numPatients() > 0){
               Patient prevPatient = myPQueue.m_heap->getPatient();
               int prevPatientPriority = myPQueue.m_priorFunc(prevPatient);
               Patient currPatient = myPQueue.getNextPatient();
               int currPatientPriority = myPQueue.m_priorFunc(currPatient);
               if(prevPatientPriority < currPatientPriority){
                    return false;
               }

          }
          return true;
     }

     bool testLeftistNPL(){
          PQueue pq1(priorityFn1, MINHEAP, LEFTIST);
          PQueue pq2(priorityFn1, MINHEAP, LEFTIST);

          // Insert elements into pq1
          for (int i = 1; i <= 100; ++i) {
               Patient patient(" ",MINTEMP,MINOX,MINRR + (i % MAXRR - MINRR),MINBP,MINOPINION);
               pq1.insertPatient(patient);
          }

          // Insert elements into pq2
          for (int i = 101; i <= 200; ++i) {
               Patient patient(" ",MINTEMP,MINOX,MINRR + (i % MAXRR - MINRR),MINBP,MINOPINION);
               pq2.insertPatient(patient);
          }

          pq1.mergeWithQueue(pq2);
          return true;
     }

     bool testLeftistProperty(Node* node){
          if (node == nullptr) {
               return true;  // An empty node satisfies the property
          }

          int leftNPL;
          int rightNPL;
          if(node->m_left == nullptr){
               leftNPL = -1;
          } else {
               leftNPL = node->m_left->getNPL();
          }

          if(node->m_right == nullptr){
               rightNPL = -1;
          } else {
               rightNPL = node->m_left->getNPL();
          }

          // Check leftist heap property
          if (leftNPL < rightNPL) {
               return false;  // Property violated
          }

          // Recursive check for left and right children
          return testLeftistProperty(node->m_left) && testLeftistProperty(node->m_right);
     }

     bool testCorrectHeapAfterNewPriFn(PQueue& myPQueue){
          if(myPQueue.m_priorFunc == priorityFn1){
               myPQueue.setPriorityFn(priorityFn2,myPQueue.getHeapType());
          } else if(myPQueue.m_priorFunc == priorityFn2){
               myPQueue.setPriorityFn(priorityFn1,myPQueue.getHeapType());
          }

          if(myPQueue.getHeapType() == MINHEAP){
               return testMinHeapProperty(myPQueue);
          } else{
               return testMaxHeapProperty(myPQueue);
          }
     }

     bool testMergeEmptyWithNormal(PQueue& aQueue, PQueue& bQueue){
          aQueue.clear();
          aQueue.setPriorityFn(bQueue.getPriorityFn(),bQueue.getHeapType());
          bQueue.mergeWithQueue(aQueue);

          if(bQueue.getHeapType() == MINHEAP){
               return testMinHeapProperty(bQueue);
          } else{
               return testMaxHeapProperty(bQueue);
          }
     }

     bool testNormalCopyConstructor(PQueue& myPQueue){
          PQueue newPQueue(myPQueue);

          while (newPQueue.m_size > 0 && myPQueue.m_size > 0){
               if(!(newPQueue.getNextPatient() == myPQueue.getNextPatient())){
                    return false;
               }
          }
          return true;
     }

     bool testEdgeCopyConstructor(PQueue& myPQueue){
          myPQueue.clear();
          PQueue newPQueue(myPQueue);

          while (newPQueue.m_size > 0 && myPQueue.m_size > 0){
               if(!(newPQueue.getNextPatient() == myPQueue.getNextPatient())){
                    return false;
               }
          }
          return true;
     }

     bool testNormalAssignmentOperator(PQueue& aQueue, PQueue& bQueue){
          aQueue = bQueue;

          while (aQueue.m_size > 0 && bQueue.m_size > 0){
               if(!(aQueue.getNextPatient() == bQueue.getNextPatient())){
                    return false;
               }
          }
          return true;
     }

     bool testEdgeAssignmentOperator(PQueue& aQueue){
          aQueue = aQueue;

           if(aQueue.getHeapType() == MINHEAP){
               return testMinHeapProperty(aQueue);
          } else{
               return testMaxHeapProperty(aQueue);
          }
     }

     bool testDequeueEmptyQueue() {
     // Create an empty PQueue instance
          PQueue emptyQueue(priorityFn1, MINHEAP, LEFTIST);
          emptyQueue.clear();
          try {
               // Attempt to dequeue from an empty queue
               Patient p = emptyQueue.getNextPatient();

               // If no exception is thrown, the test fails
               return false;
          } catch (const out_of_range& e) {
               // Test passes if out_of_range exception is caught
               return true;
          } catch (const exception& e) {
               // If any other exception is caught, the test fails
          return false;
          }
     }

     bool testMergeWithDifferentPriorityFunctions() {
     // Create an empty PQueue instance
          PQueue aQueue(priorityFn1, MINHEAP, LEFTIST);
          PQueue bQueue(priorityFn2, MINHEAP, LEFTIST);
          try {
               // Attempt to dequeue from an empty queue
               aQueue.mergeWithQueue(bQueue);

               // If no exception is thrown, the test fails
               return false;
          } catch (const domain_error& e) {
               // Test passes if out_of_range exception is caught
               return true;
          } catch (const exception& e) {
               // If any other exception is caught, the test fails
          return false;
          }
     }



};

int main(){
     Tester tester;
     PQueue aQueue(priorityFn1,MINHEAP,SKEW);
     PQueue bQueue(priorityFn1,MAXHEAP,SKEW);
     tester.insertForTests(aQueue);
     if(tester.testMinHeapProperty(aQueue)){
          cout << "Test msg: MinHeap Property preservation test after 300 insertions passed!\n" << endl;
     } else {
          cout << "Test msg: MinHeap Property preservation test after 300 insertions failed!\n" << endl;
     }

     tester.insertForTests(bQueue);
     if(tester.testMaxHeapProperty(bQueue)){
          cout << "Test msg: MaxHeap Property preservation test after 300 insertions passed!\n" << endl;
     } else {
          cout << "Test msg: MaxHeap Property preservation test after 300 insertions failed!\n" << endl;
     }

     if(tester.testMinHeapRemovalOrder(aQueue)){
          cout << "Test msg: MinHeap test for removal in ascending order passed!\n" << endl;
     } else {
          cout << "Test msg: MinHeap test for removal in ascending order failed!\n" << endl;
     }

     if(tester.testMinHeapRemovalOrder(bQueue)){
          cout << "Test msg: MaxHeap test for removal in descending order passed!\n" << endl;
     } else {
          cout << "Test msg: MinHeap test for removal in descending order failed!\n" << endl;
     }

     aQueue.clear();
     bQueue.clear();
     tester.insertForTests(aQueue);
     tester.insertForTests(bQueue);
     if(tester.testCorrectHeapAfterNewPriFn(aQueue)){
          cout << "Test msg: correct heap test after setting a new priority function passed!\n" << endl;
     } else {
          cout << "Test msg: correct heap test after setting a new priority function failed!\n" << endl;
     }

     if(tester.testMergeEmptyWithNormal(aQueue,bQueue)){
          cout << "Test msg: correct heap test after merging an empty queue with a normal queue passed!\n" << endl;
     } else {
          cout << "Test msg: correct heap test after merging an empty queue with a normal queue failed!\n" << endl;
     }

     if(tester.testNormalCopyConstructor(aQueue)){
          cout << "Test msg: Normal test for copy constructor passed!\n" << endl;
     } else {
          cout << "Test msg: Normal test for copy constructor failed!\n" << endl;
     }
     
     bQueue.clear();
     tester.insertForTests(bQueue);
     if(tester.testEdgeCopyConstructor(aQueue)){
          cout << "Test msg: Edge test for copy constructor with an empty queue passed!\n" << endl;
     } else {
          cout << "Test msg: Edge test for copy constructor with an empty queue failed!\n" << endl;
     }

     if(tester.testNormalAssignmentOperator(aQueue,bQueue)){
          cout << "Test msg: Normal test for assignment operator passed!\n" << endl;
     } else {
          cout << "Test msg: Normal test for assignment operator failed!\n" << endl;
     }

     if(tester.testEdgeAssignmentOperator(aQueue)){
          cout << "Test msg: Edge test for assignment operator with self-assignment passed!\n" << endl;
     } else {
          cout << "Test msg: Edge test for assignment operator with self-assignment failed!\n" << endl;
     }

     if(tester.testDequeueEmptyQueue()){
          cout << "Test msg: Test for catching an out_of_range error attempting to dequeue an empty queue passed!\n" << endl;
     } else {
          cout << "Test msg: Test for catching an out_of_range error attempting to dequeue an empty queue failed!\n" << endl;
     }

     if(tester.testMergeWithDifferentPriorityFunctions()){
          cout << "Test msg: Test for attempting to merge two Pqueue's with different priority functions passed" << endl;
     } else {
          cout << "Test msg: Test for attempting to merge two Pqueue's with different priority functions failed" << endl;
     }

}

int priorityFn1(const Patient & patient) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [115-242]
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    //the larger value means the higher priority
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient & patient) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [71-111]
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    //the smaller value means the higher priority
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}