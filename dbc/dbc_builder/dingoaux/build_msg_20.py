import cantools
from utils.signal_utils import create_duplicate_signals

def build_msg_20(base_id):
    message = cantools.database.Message(
        frame_id=base_id + 20,
        name="dingoAuxMsg20",
        length=8,
        is_extended_frame=False,
        signals=[]
    )

    output_dc_sigs = create_duplicate_signals("OutputDC", 7, 9, 0, 8, 1, 0, "%")
    message.signals.extend(output_dc_sigs)
    return message
