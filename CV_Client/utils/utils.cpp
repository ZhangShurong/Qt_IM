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


AES_utils::AES_utils()
{
    unsigned char iv[] = {103,35,148,239,76,213,47,118,255,222,123,176,106,134,98,92};
    unsigned char key[] = {143,194,34,208,145,203,230,143,177,246,97,206,145,92,255,84};
    moo.set_key(key);
    moo.set_mode(MODE_OFB);
    moo.set_iv(iv);
}

int AES_utils::encrypt(unsigned char *in, size_t src_len, unsigned char *out)
{

    char *temp = new char[src_len];
    memcpy(temp, in, src_len);
    int len = moo.Encrypt((unsigned char *)temp, src_len, (unsigned char *)out);
    return len;

}

int AES_utils::decrypt(unsigned char *in, size_t src_len, unsigned char *out)
{
    int len = moo.Decrypt((unsigned char *)in, src_len, (unsigned char *)out);
    return len;
}
//加密
void encode(char *pstr){
    int len = strlen(pstr);//获取长度
    for (int i = 0; i < len; i++)
        *(pstr + i) = *(pstr + i) ^ i;
}

//解密
void decode(char *pstr){
    int len = strlen(pstr);
    for (int i = 0; i < len; i++)
        *(pstr + i) = *(pstr + i) ^ i;
}
