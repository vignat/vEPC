/*
 * Copyright 2017-present Open Networking Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/********************************************************************20**

     Name:     eGTP Tool for Code Generation

     Type:     C++ Header file

     Desc:     This file contains the classes used by egcodetool

     File:     egcodetool.h

     Sid:      egcodetool.h@@/main/3 - Fri Feb  4 18:18:19 2011

     Prg:      an 

*********************************************************************21*/

class RuntimeStringCmp 
{
 public: 
    enum cmp_mode { normal, nocase };
 private:
    const cmp_mode mode;
    static bool nocase_comp(char c1, char c2)
    {
       return toupper(c1) < toupper(c2);
    }
 public:
   RuntimeStringCmp (cmp_mode m=nocase) : mode(m)
   {
   }

   bool operator() (const string& s1, const string& s2) const
   {
      if (mode == normal)
         return(s1<s2);
      else
      {
         return lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),
                                 nocase_comp);
      }
   }
}; //end of RuntimeStringCmp class

void hdefine(char& c)
{
   if (c == '-')
   {
           c = '_';
   }
   else
        {
           c = toupper(c);
        }
} // end of hdefine function

typedef struct
{
    string enumType;
    string enumVal;
} EnumElem;



// This class represents an IE in the IE dictionary File
class Ie
{
 private:
   string     ieCode;
   string     ieType; 
   string     ieDataType;
   string     ieFuncPtr;
   string     defName;
   string     minLen;
   string     maxLen;
   deque<EnumElem> enumQ;
 public: 
   string type() {return ieType;}
   string dataType() {return ieDataType;}
   string code() {return ieCode;}
        string name(){return defName;}
   string funcPtr() {return ieFuncPtr;}
   string getMinLen() { return minLen;}
   string getMaxLen() { return maxLen;}
   void addVal(EnumElem&);
   int getSize() { return enumQ.size(); }
   EnumElem getVal(int idx) { return enumQ.at(idx); }
   Ie(string& type, string& code, string& dataType,string& funcPtr, string& minLen, string& maxLen);
};

class MessageIe
{
private: 
   string name;
   string ieClass;
        string ieType;
        string pos;
        string instance;
        string isList;
public: 
   MessageIe(string& _name, string& _ieClass, string& _ieType, string& _pos, string& _instance, string& _isList);
   string getName() { return name;}
   string getIeClass() { return ieClass;}
   string getIeType() { return ieType;}
   string getPos() { return pos;}
   string getInstance() { return instance;}
   string getIsList() { return isList;}
};

class SubIe
{
private:
        string subIe;
        string msgName;
        string groupedIe;
        string subGroupedIe;
        string ieClass;
        string grInst;
        string subGrpInst;
        string instance;
        string isList;
public:
   SubIe(string& _subIe, string& _msgName, string& _groupedIe, string& _subGroupedIe, string& _ieClass, string& _grInst, string& _subGrpInst, string& _instance, string& _isList);
   string getMsgName() {return msgName;}
   string getGroupedIe() {return groupedIe;}
   string getSubIe() {return subIe;}
   string getSubGroupedIe() { return subGroupedIe;}
   string getIeClass() {return ieClass;}
   string getGrInst() {return grInst;}
   string getSubGrInst() {return subGrpInst;}
   string getInstance() {return instance;}
   string getIsList() {return isList;}
};

typedef Ie* IePtr;
typedef SubIe* SubIePtr;
typedef MessageIe* MessageIePtr;
typedef deque<SubIePtr>* listSubIePtr;
typedef deque<MessageIePtr> MessageIeList;

// This class represents a GTP Message and contains references to all IEs in that Message
class Message
{
private:
   MessageIeList ieList;
   string msgCode;
   string funcPtr;
   string defName;
public:
   Message(string& _name, string& _msgCode, string& _funcPtr);
   string name(){return defName;}
   string code(){return msgCode;}
   string getfuncPtr(){return funcPtr;}
   MessageIePtr ie(int idx)
   {
      return(ieList.at(idx));
   }
   int numIe() { return(ieList.size());}

   void add(MessageIePtr iePtr)
   {
      ieList.push_back(iePtr);
   }
};

typedef Message* GmPtr;

class OutFile
{
private: 
   ofstream fp;
   string filename;
   string desc;
public:
   void addHeader();
   void addFooter();
   void add(ostringstream& o)
   {
      fp << o.str() << endl;;   
   }
   void add(string s)
   {
      fp << s << endl;
   }
   void add(char *s)
   {
      fp << s << endl;
   }
   void bl() {fp << endl;}
   void bl(unsigned int cnt)
   {
      while(cnt)
      {
         fp << endl;
         cnt--;
      }
   }
   void bsc()
   {
      fp << "/***********************************************************************"  << endl;
   }
   void bec()
   {
      fp << "************************************************************************/"  << endl;
   }

   OutFile(string _filename, const char* _desc)
   {
      filename = _filename;
      desc = _desc;
      fp.open(filename.c_str());

        if (!fp)
        {
           cerr << "can't open output file for writing" << filename << endl;
           throw 0;
        }
      addHeader();
      bsc();
      add("    This file contains automatically generated code");
      bec();
   }

   void addFuncHeader(string funcName, string purpose, string ret);

   ~OutFile()
   {
      addFooter();
   }
};

typedef map<string, IePtr, RuntimeStringCmp> IeMap;
typedef multimap<string, IePtr, RuntimeStringCmp> IeMultiMap;

void egParseIEFile(void);
void egParseGMFile(const char *);
void egGenOutputFiles(char *prefix);

/********************************************************************30**

         End of file:     egcodetool.h@@/main/3 - Fri Feb  4 18:18:19 2011

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      ad                1. Created for Initial Release 1.1
/main/2      ---      rsharon  1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla         Initial for eGTP release 2.1
*********************************************************************91*/
