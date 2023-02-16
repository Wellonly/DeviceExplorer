
function dbInit()
{
    var db = LocalStorage.openDatabaseSync("UI_Activity_Tracker_DB", "", "UI Track example", 1000000)
    try {
        db.transaction(function (tx) {
            tx.executeSql('CREATE TABLE IF NOT EXISTS ui_log_table (event_date text,event_time numeric,event_value numeric)')
        })
    } catch (err) {
        console.log("Error creating table in database: " + err)
    };
}

function dbGetHandle()
{
    try {
        var db = LocalStorage.openDatabaseSync("UI_Activity_Tracker_DB", "", "UI Track example", 1000000)
    } catch (err) {
        console.log("Error opening database: " + err)
    }
    return db
}

function dbInsert(Pdate, Ptime, Pvalue)
{
    var db = dbGetHandle()
    var rowid = 0;
    db.transaction(function (tx) {
        tx.executeSql('INSERT INTO ui_log_table VALUES(?, ?, ?)', [Pdate, Ptime, Pvalue])
        var result = tx.executeSql('SELECT last_insert_rowid()')
        rowid = result.insertId
    })
    return rowid;
}

function dbReadAll()
{
    var db = dbGetHandle()
    db.transaction(function (tx) {
        var results = tx.executeSql(
                    'SELECT rowid,event_date,event_time,event_value FROM ui_log_table order by rowid desc')
        for (var i = 0; i < results.rows.length; i++) {
            listModel.append({
                                 id: results.rows.item(i).rowid,
                                 checked: " ",
                                 event_date: results.rows.item(i).event_date,
                                 event_time: results.rows.item(i).event_time,
                                 event_value: results.rows.item(i).event_value
                             })
        }
    })
}

function dbUpdate(Pdate, Ptime, Pvalue, Prowid)
{
    var db = dbGetHandle()
    db.transaction(function (tx) {
        tx.executeSql(
                    'update ui_log_table set event_date=?, event_time=?, event_value=? where rowid = ?', [Pdate, Ptime, Pvalue, Prowid])
    })
}

function dbDeleteRow(Prowid)
{
    var db = dbGetHandle()
    db.transaction(function (tx) {
        tx.executeSql('delete from ui_log_table where rowid = ?', [Prowid])
    })
}

function dbDeleteAll()
{
    var db = dbGetHandle()
    db.transaction(function (tx) {
        tx.executeSql('delete from ui_log_table')
    })
}
