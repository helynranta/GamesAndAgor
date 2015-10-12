
class Message {
public:
  inline Message() {}
  inline virtual ~Message() {;}
  virtual void unpack() = 0;
  virtual void pack() = 0;
  virtual void update() = 0;
};

class NickACK : public Message {
public:
  inline NickACK(/* byttejä tänn */) {;}
  inline ~NickACK() {;}
  inline void unpack() override {
    ;
  }
  inline void pack() override {
    ;
  }
  inline void update() override {
      ;
  }
};
