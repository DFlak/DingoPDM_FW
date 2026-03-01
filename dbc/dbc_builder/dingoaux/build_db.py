
import cantools
import dingoaux

def build_db(base_id):
    db = cantools.database.Database()
    db.name = "dingoAux"
    db.version = "0.1.0"
    
    db.messages.append(dingoaux.build_msg_0(base_id))
    db.messages.append(dingoaux.build_msg_1(base_id))
    db.messages.append(dingoaux.build_msg_2(base_id))
    db.messages.append(dingoaux.build_msg_3(base_id))
    db.messages.append(dingoaux.build_msg_4(base_id))
    db.messages.append(dingoaux.build_msg_5(base_id))
    db.messages.append(dingoaux.build_msg_6(base_id))
    db.messages.append(dingoaux.build_msg_7(base_id))
    db.messages.append(dingoaux.build_msg_8(base_id))
    db.messages.append(dingoaux.build_msg_9(base_id))
    db.messages.append(dingoaux.build_msg_10(base_id))
    db.messages.append(dingoaux.build_msg_11(base_id))
    db.messages.append(dingoaux.build_msg_12(base_id))
    db.messages.append(dingoaux.build_msg_13(base_id))
    db.messages.append(dingoaux.build_msg_14(base_id))
    db.messages.append(dingoaux.build_msg_15(base_id))
    db.messages.append(dingoaux.build_msg_16(base_id))
    db.messages.append(dingoaux.build_msg_17(base_id))
    db.messages.append(dingoaux.build_msg_18(base_id))
    db.messages.append(dingoaux.build_msg_19(base_id))
    db.messages.append(dingoaux.build_msg_20(base_id))

    return db
