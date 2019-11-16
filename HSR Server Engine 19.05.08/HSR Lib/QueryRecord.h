#pragma once

class QueryRecord
{
    recordPtr		record_;
	int				resultVal_;		//쿼리 결과값
public:	
    QueryRecord();
    ~QueryRecord();

    void errorReport(_com_error &e);

    void operator = (QueryRecord &lvalue);
    void operator = (recordPtr &lvalue);
	void operator = (recordPtr && rvalue);  // 다시 볼것
    recordPtr &resultRecord();

    bool        opened();
    void        close();
    void        setRecord(recordPtr record);
	bool		isEof();

	int			resultVal();
	void		setResultVal(int result);

	HRESULT		moveNext();
	HRESULT		movePrevious();
	HRESULT		moveFirst();
	HRESULT		moveLast();

	bool		get(const char* fieldName, char* fieldValue);
	bool		get(const char* fieldName, wchar_t* fieldValue);
	bool		get(const char* fieldName, int32_t& fieldValue);
	bool		get(const char* fieldName, int64_t& fieldValue);
	bool		get(const char* fieldName, float& fieldValue);
	bool		get(const char* fieldName, double& fieldValue);
	bool		get(const char* fieldName, long& fieldValue);
};
