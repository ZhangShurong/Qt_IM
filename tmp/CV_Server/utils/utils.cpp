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
        }
    }
    return jspp_msg;
}
