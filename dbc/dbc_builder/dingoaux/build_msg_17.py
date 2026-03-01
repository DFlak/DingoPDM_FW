import cantools
from utils.signal_utils import create_duplicate_signals

def build_msg_17(base_id):
    message = cantools.database.Message(
        frame_id=base_id + 17,
        name="dingoAuxMsg17",
        length=8,
        is_extended_frame=False,
        signals=[]
    )

    output_current_sigs = create_duplicate_signals("OutputCurrent", 3, 13, 0, 16, 0.1, 0, "A")
    message.signals.extend(output_current_sigs)
    return message
