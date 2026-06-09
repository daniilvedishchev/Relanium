#property strict
#property version "1.00"

const string ServerHost = "127.0.0.1";
uint ServerPort = 8080;
uint TimeoutMs = 1000;
uint TimerMs = 200;

enum SignalSide {
   SIGNAL_BUY,
   SIGNAL_SELL,
   SIGNAL_UNKNOWN
};

struct Signal {
   string SYMBOL;
   SignalSide SIDE;
   double QTY;
   double SL;
   double TP;
};

int handle = INVALID_HANDLE;

void OnTimer(){
   if (handle == INVALID_HANDLE){
      return;
   }
   int bytes = SocketIsReadable(handle);
   if (bytes<=0) return;
   
   uchar buffer[];
   ArrayResize(buffer,bytes);
   
   int bytesRead = SocketRead(handle,buffer,bytes,TimeoutMs);
   if (bytesRead<0) {
      Print(GetLastError()); 
      return;
   }
   
   if (bytesRead == 0) return;
   
   string msg = CharArrayToString(buffer,0,bytesRead);
   HandleSignal(msg);
}

void HandleSignal(string msgSignal){
   Signal signal{};
   
   string splittedStringArray[];
   ushort separator = StringGetCharacter(" ",0);
   
   StringSplit(msgSignal,separator,splittedStringArray);
   
   if (ArraySize(splittedStringArray) != 5){
      Print("UB: invalid signal format.");
      return;
   }
   
   resolveSignal(signal,splittedStringArray);
   if (!validateSignal(signal)){
      return;
   }
   
   printSignal(signal);
}


SignalSide resolveSide(string& side){
   if (side == "BUY") return SIGNAL_BUY;
   if (side == "SELL") return SIGNAL_SELL;
   return SIGNAL_UNKNOWN;
}

void resolveSignal(Signal& signal, string& splitted[]){
   signal.SYMBOL = splitted[0];
   signal.SIDE = resolveSide(splitted[1]);
   signal.QTY = StringToDouble(splitted[2]);
   signal.SL = StringToDouble(splitted[3]);
   signal.TP = StringToDouble(splitted[4]);
}

bool validateSignal(Signal& sig){
   if (StringLen(sig.SYMBOL)<=0) return false;
   if (sig.SIDE == SIGNAL_UNKNOWN) return false;
   if (sig.QTY <= 0) return false;
   if (sig.SL <= 0) return false;
   if (sig.TP <= 0) return false;
   return true;
}
  

void printSignal(Signal& sig){
   Print("**** Incoming order from Oraculum ****");
   Print("SYMBOL:",sig.SYMBOL);
   Print("SIDE:",sig.SIDE);
   Print("QTY:",sig.QTY);
   Print("SL:",sig.SL);
   Print("TP:",sig.TP);
}

int OnInit(){
    handle = SocketCreate();
    if (handle == INVALID_HANDLE){
      Print(GetLastError());
      return INIT_FAILED;
    } else {
      bool connected = SocketConnect(handle,ServerHost,ServerPort,TimeoutMs);
      if (!connected){
         close();
         return INIT_FAILED;
      } else {
         bool isTimerSet = EventSetMillisecondTimer(TimerMs);
         if (!isTimerSet){
            close();
            return INIT_FAILED;
         }
         return INIT_SUCCEEDED;
      }
    }
}

void close(){
   SocketClose(handle);
   handle = INVALID_HANDLE;
}

void OnDeinit(const int reason){
    Print("EA has stopped.");
    if (handle != INVALID_HANDLE){
      close();
      Print("Socket was successfully closed.");
    }
}