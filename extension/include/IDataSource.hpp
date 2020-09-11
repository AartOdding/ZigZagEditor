#pragma once


// void getPluginDataSources()


class IDataSource
{
public:


};


class IDataSourceFactory
{
public:

    virtual IDataSource * createInstance() = 0;

    virtual void destroyInstance(IDataSource * instance) = 0;

    virtual void getLayout(int* count, const char** functions) = 0;

};



