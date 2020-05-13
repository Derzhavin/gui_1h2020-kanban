#include "sqltablemodelextension.h"

void foreachRecord(QSqlTableModel& model, std::function<void(QSqlRecord& record)> callback) {
    for (quint8 i = 0; i < model.rowCount(); ++i) {
        QSqlRecord record = model.record(i);
        callback(record);
        model.setRecord(i, record);
    }
}
