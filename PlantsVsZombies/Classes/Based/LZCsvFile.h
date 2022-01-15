//
//  CSVFile.h
//
//  Created by quasi_lee on 15-7-7.
//  Modify by lz on 22-1-14
//

#ifndef __CSVFile__
#define __CSVFile__

#include"cocos2d.h"

using namespace cocos2d;
using namespace std;

class CSVFile {
public:
    CSVFile();
    ~CSVFile();

    //���Դ洢����
    vector<vector<string> > data;

public:
    //��CSV�ļ�
    bool openFile(const char* fileName);
    bool openFile(string& str);
    //�������л�ȡ����
    const char* getData(int rows, int cols);
    //��ȡָ�����ݵ����±�
    int findColsData(int cols, const char* value);
    //�õ�������
    inline int getCols() { return cols; }
    //�õ�������
    inline int getRows() { return data.size(); }
    //���������
    void addNewData(vector<string>& newData);
    //��������,posΪ����ʱ������λ�ã�cmpĬ�Ͻ���
    void sortData(int pos, bool cmp = false);

private:
    void StringSplit(const string& str, vector<string>& tokens, const char& delimiters);
    void split(vector<string>& field, string line);
    int advplain(const string& line, string& fld, int);
    int advquoted(const string& line, string& fld, int);

private:
    string fieldsep;
    int cols;
};

#endif /* defined(__CSVFile__) */