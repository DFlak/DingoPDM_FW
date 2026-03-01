import cantools
from utils.signal_utils import create_duplicate_signals

def build_msg_18(base_id):
    message = cantools.database.Message(
        frame_id=base_id + 18,
        name="dingoAuxMsg18",
        length=8,
        is_extended_frame=False,
        signals=[]
    )

    output_states = {
        0: "Off",
        1: "On",
        2: "Overcurrent",
        3: "Fault"
    }
    output_state_sigs = create_duplicate_signals("OutputState", 7, 9, 0, 4, 1, 0)
    for sig in output_state_sigs:
        sig.choices = output_states
    message.signals.extend(output_state_sigs)

    return message
