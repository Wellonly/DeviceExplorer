import QtQuick 2.7
import QtQuick.Extras 1.4

Tumbler {
    TumblerColumn {
        model: [1, 2, 3]
    }
    
    TumblerColumn {
        model: ["A", "B", "C"]
        visible: true
    }

    TumblerColumn {
        model: [-1, 0, 1]
        visible: true
    }
}
