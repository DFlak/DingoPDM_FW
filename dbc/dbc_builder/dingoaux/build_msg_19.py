import cantools
from utils.signal_utils import create_duplicate_signals

def build_msg_19(base_id):
    message = cantools.database.Message(
        frame_id=base_id + 19,
        name="dingoAuxMsg19",
        length=8,
        is_extended_frame=False,
        signals=[]
    )

    output_reset_count_sigs = create_duplicate_signals("OutputResetCount", 7, 9, 0, 8, 1, 0)
    message.signals.extend(output_reset_count_sigs)
    return message
