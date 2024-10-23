import debug_funcs
from debug_cli import CmdLine
import numpy as np
import pytest


def test_scoped_eval_numeric_type():
    ret = debug_funcs.scoped_eval("16")
    assert ret == 16

    ret = debug_funcs.scoped_eval("(5+6)*0.75+34")
    assert ret == (5 + 6) * 0.75 + 34


def test_scoped_eval_float_arrays_type():
    ret = debug_funcs.scoped_eval("lin(1,4)")
    assert (ret == np.linspace(1, 4, 11)).all()

    ret = debug_funcs.scoped_eval("eigen()")
    assert (ret == np.linspace(0, 1, 11)).all()

    ret = debug_funcs.scoped_eval("eigen()*1.25+2")
    assert (ret == np.linspace(0, 1, 11) * 1.25 + 2).all()

    ret = debug_funcs.scoped_eval(
        "[1, 2, 3, 4, 5.5, 4.3, 2.3, 62.1, 64, 19.111111, 15.77]"
    )
    assert (
        ret == np.array([1, 2, 3, 4, 5.5, 4.3, 2.3, 62.1, 64, 19.111111, 15.77])
    ).all()

    ret = debug_funcs.scoped_eval("(np.arange(11)+15.023-np.pi)*1.5")
    assert (ret == (np.arange(11) + 15.023 - np.pi) * 1.5).all()


def test_scoped_eval_undefined_function_error():
    with pytest.raises(NameError):
        debug_funcs.scoped_eval("undefined_func_abc()")


def test_scoped_eval_incorrect_function_usage():
    with pytest.raises(TypeError):
        debug_funcs.scoped_eval("lin()")

    with pytest.raises(TypeError):
        debug_funcs.scoped_eval("lin(1)")

    with pytest.raises(TypeError):
        debug_funcs.scoped_eval("lin(1,4,6)")

    with pytest.raises(TypeError):
        debug_funcs.scoped_eval("eigen(3,5)")


def test_float_array_analysis():
    cmd = CmdLine(None)
    ret = cmd.float_array_analysis(
        [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
    )
    assert "linear" in ret

    # Do to inherent imprecision of floats, we use (expected-value)/expected =< 1e-6 to assert a value is equal
    ret = cmd.float_array_analysis(
        [0, 0.1000000001, 0.2000000000000151, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1]
    )
    assert "linear" in ret

    ret = cmd.float_array_analysis(
        [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.00001]
    )
    assert "linear" not in ret

    ret = cmd.float_array_analysis(
        [0.0, 0.1, 0.2, 0.3, 0.4, 0.5020, 0.6, 0.7, 0.8, 0.9, 1.0]
    )
    assert "linear" not in ret
