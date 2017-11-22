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
