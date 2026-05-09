import QtQuick


Item {
    id:root

    property alias model:boardModel
    property bool boardReady:false
    property int clearPictureId: -1
    property int maxBoardSize: 12

    signal allCellsClear()

    ListModel{
        id:boardModel
        ListElement{ index: 0;  picture_id:-1; selected:false}
    }

    Component.onCompleted: {
        initBoard("1#2#3#4#5#6#6#5#4#3#2#1")
        isBoardClear()

    }

    function initBoard( init_data ) {
        boardModel.clear();

        const pictureIndex = init_data.split("#", root.maxBoardSize);
        if ( inRange( 0, pictureIndex.length, root.maxBoardSize )  ) {
            for (var i = 0; i < pictureIndex.length; i++) {
                //console.log(`i:${i}, parseInt(pictureIndex[i],10): ${parseInt(pictureIndex[i],10)} `)
                boardModel.append({"index": i, "picture_id":parseInt(pictureIndex[i],10), selected:false})
            }
            root.boardReady = true
        }else{
            root.boardReady = false
        }
    }

    function inRange (num, min, max) {
        return (num, min, max) => num >= min && num <= max;
    }

    function isSelectedCellsPair(firstIndex, secondIndex){

        if ( inRange( firstIndex, 0, boardModel.count )  &&
                inRange( secondIndex, 0, boardModel.count ) &&
                ( firstIndex != secondIndex ) ) {

            let firstPictureId = boardModel.get(firstIndex).picture_id;
            let secondPictureId = boardModel.get(secondIndex).picture_id;

            if ( ( firstPictureId  === secondPictureId ) &&
                    ( firstPictureId != root.clearPictureId) || ( secondPictureId != root.clearPictureId)  ){
                return true
            }
        }
        return false
    }

    function isBoardClear(){
        console.log(`BoardModel.count ${boardModel.count}`)
        for (var i = 0; i < boardModel.count; i++) {
            var row = boardModel.get(i);
            //console.log(`Current row ${i}: index: ${row.index}, picture_id:${row.picture_id}, selected: ${row.selected}`)
            if ( row.picture_id != root.clearPictureId ) return false;
        }
        return true;
    }

}
