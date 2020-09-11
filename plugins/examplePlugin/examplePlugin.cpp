#include <ZigZag/IDataSource.hpp>
#include <ZigZag/Platform.hpp>


class ExampleDataSource : public IDataSource
{
public:

    int squareInt(int number);
    float squareFloat(float number);

};

int ExampleDataSource::squareInt(int number)
{
    return number * number;
}

float ExampleDataSource::squareFloat(float number)
{
    return number * number;
}


class ExampleDataSourceFactory : public IDataSourceFactory
{
public:
    
    IDataSource * createInstance()
    {
        return new ExampleDataSource();
    }

    void destroyInstance(IDataSource * instance)
    {
        if (instance)
        {
            delete instance;
        }
    }

    void getLayout(int* count, const char** functions)
    {
        static const char* f[] = {
            "int squareInt(int number);",
            "float squareFloat(float number);"
        };
        *count = 1;
        *functions = f[0];
    }

};


ExampleDataSourceFactory exampleFactory;

ZIGZAG_C_API void getPluginDataSources(int* count, IDataSourceFactory** dataSourceFactories)
{
    *count = 1;
    *dataSourceFactories = &exampleFactory;
}

