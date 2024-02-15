// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"
/*This is the constructor. It must be provided with a 
pointer to the prioritization function, the type of heap, 
and the desired data structure.*/
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure){
  m_heapType = heapType;
  m_structure = structure;
  m_priorFunc = priFn;
  m_size = 0;
  m_heap = nullptr;
}

/*The destructor deallocates the memory and re-initializes 
the member variables.*/
PQueue::~PQueue(){
  if(m_heap != nullptr){
    clear();
    m_heap = nullptr;
  }
  
}

void PQueue::clear(){
  if (m_heap != nullptr){
    deleteTree(m_heap);
    m_size = 0;
  }
  
  
}

void PQueue::deleteTree(Node* pat){
  if (pat != nullptr){
    deleteTree(pat->m_left);
    deleteTree(pat->m_right);
    delete pat;
    pat = nullptr;
  } 
}

/*The copy constructor must make a deep copy of the rhs object. 
It must function correctly if rhs is empty. This function creates 
an exact same copy of rhs.*/
PQueue::PQueue(const PQueue& rhs){
  m_heapType = rhs.getHeapType();
  m_structure = rhs.getStructure();
  m_priorFunc = rhs.getPriorityFn();

  if (rhs.m_heap != nullptr){
    m_heap = deepCopyHeap(rhs.m_heap);
    m_size = rhs.m_size;
  } else {
    m_heap = nullptr;
    m_size = 0;
  }
}

Node* PQueue::deepCopyHeap(Node* node){
  if (node == nullptr){
    return nullptr;
  }

  Node* newNode = new Node(node->getPatient());
  newNode->setNPL(node->getNPL());

  newNode->m_left = deepCopyHeap(node->m_left);
  newNode->m_right = deepCopyHeap(node->m_right);
  
  return newNode;
  
}
PQueue& PQueue::operator=(const PQueue& rhs) {
  if (this != &rhs) {
    // Clear the current object
    clear();

    // Initialize member variables
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_priorFunc = rhs.m_priorFunc;

    // Perform deep copy
    if (rhs.m_heap != nullptr) {
      m_heap = deepCopyHeap(rhs.m_heap);
      m_size = rhs.m_size;
    } else {
      // If rhs is empty, set the heap to nullptr and size to 0
      m_heap = nullptr;
      m_size = 0;
    }
  }

  return *this;
}

void PQueue::mergeWithQueue(PQueue& rhs) {
  if(this == &rhs){
    throw domain_error("Merging a heap with itself is not allowed");
  }

  if(m_priorFunc != rhs.m_priorFunc){
    throw domain_error("PQueues must have the same priority function");
  }

  if(m_structure != rhs.m_structure){
    throw domain_error("PQueues must have the same structure");
  }

  m_heap = merge(m_heap,rhs.m_heap);
  m_size += rhs.m_size;
  rhs.m_heap = nullptr;
  rhs.m_size = 0;
}

void PQueue::insertPatient(const Patient& patient) {
  Node* newNode = new Node(patient);
  if(m_heap != nullptr){
    m_heap = merge(m_heap,newNode);
  } else {
    m_heap = newNode;
  }

  m_size++;
}

Node* PQueue::merge(Node* aHeap, Node* bHeap){
  if(aHeap == nullptr){
    return bHeap;
  }
  
  if(bHeap == nullptr){
    return aHeap;
  }
  if (aHeap != nullptr && bHeap != nullptr){
    if ((m_heapType == MINHEAP && m_priorFunc(aHeap->getPatient()) > m_priorFunc(bHeap->getPatient())) ||
        (m_heapType == MAXHEAP && m_priorFunc(aHeap->getPatient()) < m_priorFunc(bHeap->getPatient()))) {
      Node* temp = aHeap;
      aHeap = bHeap;
      bHeap = temp;
    }

    if (m_structure == LEFTIST) {
      aHeap->m_right = merge(aHeap->m_right, bHeap);

      if (aHeap->m_left == nullptr || aHeap->m_left->getNPL() < aHeap->m_right->getNPL()) {
          Node* temp = aHeap->m_left;
          aHeap->m_left = aHeap->m_right;
          aHeap->m_right = temp;
      }

      if(aHeap->m_right == nullptr){
        aHeap->setNPL(0);
      } else {
        aHeap->setNPL(min(aHeap->m_left->getNPL(),aHeap->m_right->getNPL()+1));
      }

      
      if(aHeap->m_left == nullptr || aHeap->m_right == nullptr){
        aHeap->setNPL(0);
      } else{
        if(aHeap->m_left->getNPL() <= aHeap->m_right->getNPL()){
          aHeap->setNPL(aHeap->m_left->getNPL() + 1);
        }
        else {
          aHeap->setNPL(aHeap->m_right->getNPL() + 1);
        }
      }

    }else if (m_structure == SKEW) {
      swap(aHeap->m_left,aHeap->m_right);
      aHeap->m_left = merge(aHeap->m_left, bHeap);
    }

    
  }
  return aHeap;
  
}

int PQueue::numPatients() const{
  return m_size;
}

prifn_t PQueue::getPriorityFn() const {
  return m_priorFunc;
}

Patient PQueue::getNextPatient() {
  if (m_heap == nullptr){
    throw out_of_range("PQueue is Empty");
  }

  Patient highestPriorityPat = m_heap->getPatient();

  Node* currentRoot = m_heap;
  m_heap = merge(m_heap->m_left,m_heap->m_right);
  m_size--;

  delete currentRoot;

  return highestPriorityPat;
}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  PQueue temp(priFn,heapType,m_structure);

  storePatients(m_heap,temp);

  clear();

  m_priorFunc = priFn;
  m_heapType = heapType;
  while(temp.numPatients() > 0){
    insertPatient(temp.getNextPatient());
  }
}

void PQueue::storePatients(Node* pat, PQueue& temp) const{
  //cout << "run1" << endl;
  if(pat == nullptr){
    return;
  }else if(pat != nullptr){
    //cout << "run2" << endl;
    temp.insertPatient(pat->getPatient());
    //cout << "run3" << endl;
    storePatients(pat->m_left,temp);
    storePatients(pat->m_right,temp);
  }
}

void PQueue::setStructure(STRUCTURE structure){
  if(m_structure != structure){
    PQueue temp(m_priorFunc,m_heapType,structure);
    
    if(m_heap != nullptr){
      while(m_size > 0){
        temp.insertPatient(getNextPatient());
      }
      m_heap = temp.m_heap;//merge(m_heap,temp.m_heap);
      m_size = temp.m_size;
    }
    

    

    /*clear();

    m_structure = structure;
    while(temp.numPatients() > 0){
      insertPatient(temp.getNextPatient());
    }
  } else if (m_structure == structure){
    return;
  */
  }
}

STRUCTURE PQueue::getStructure() const {
  return m_structure;
}

HEAPTYPE PQueue::getHeapType() const {
  return m_heapType;
}

void PQueue::printPatientQueue() const {
  if(m_heap != nullptr){
    displayHeap(m_heap);
  }
  
}

void PQueue::displayHeap(Node* pat) const{
  if(pat != nullptr){
    cout << "[" << m_priorFunc(pat->getPatient()) << "] "  << pat->getPatient() << endl;
    displayHeap(pat->m_left);
    displayHeap(pat->m_right);
  }
}

void PQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void PQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
    else
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout  << patient.getPatient() 
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}
