#include "utils.h"
#include <QJsonObject>
#include <QJsonDocument>
JSPP parse(string json_str)
{
    JSPP jspp_msg;
    QString json_qstr = QString::fromStdString(json_str);
    //简单的QT解析JSON
    QJsonParseError simp_json_error;
    //QTJSON所有的元素
    QJsonDocument simp_parse_doucment = QJsonDocument::fromJson(json_qstr.toUtf8(), &simp_json_error);
    //检查json是否有错误
    if (simp_json_error.error == QJsonParseError::NoError)
    {
        if (simp_parse_doucment.isObject())
        {
            //开始解析json对象
            QJsonObject obj = simp_parse_doucment.object();
            jspp_msg.type = obj["type"].toString().toStdString();
            jspp_msg.to = obj["to"].toString().toStdString();
            jspp_msg.from = obj["from"].toString().toStdString();
            jspp_msg.body = obj["body"].toString().toStdString();
            jspp_msg.code = obj["code"].toString().toStdString();
        }
    }
    return jspp_msg;
}

string jspp_to_str(const JSPP msg_json)
{
    QJsonObject json;
    json.insert(QString("type"), QString::fromStdString(msg_json.type));
    json.insert(QString("to"), QString::fromStdString(msg_json.to));
    json.insert(QString("from"), QString::fromStdString(msg_json.from));
    json.insert(QString("body"), QString::fromStdString(msg_json.body));
    json.insert(QString("code"), QString::fromStdString(msg_json.code));
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    return strJson.toStdString();
}
//加密
void encode(char *pstr, int len_a){
    int len;
    if(len_a == -1)
        len = strlen(pstr);//获取长度
    else
        len = len_a;
    for (int i = 0; i < len; i++)
        *(pstr + i) = *(pstr + i) ^ i;
}

//解密
void decode(char *pstr,int len_a){
    int len;
    if(len_a == -1)
        len = strlen(pstr);//获取长度
    else
        len = len_a;
    for (int i = 0; i < len; i++)
        *(pstr + i) = *(pstr + i) ^ i;
}

QString encodeQstr(QString src)
{
    char *tmp = new char[src.size()];
    memcpy(tmp, src.toStdString().c_str(),src.size());
    encode(tmp,src.size());
    QString res;
    QString c = "";
    for(int i = 0; i < src.size(); ++i)
    {
        res += c + QString::number((int)tmp[i]);
        c = "&";
    }
    delete []tmp;
    return res;
}

QString decodeQstr(QString src)
{
    QStringList input =  src.split("&");
    char *input_arr = new char[input.size()];
    for(int i = 0; i < input.size(); ++i) {
        input_arr[i] = input[i].toInt();
    }
    decode(input_arr, input.size());
    QString res(input_arr);
    delete[] input_arr;
    return res;
}
