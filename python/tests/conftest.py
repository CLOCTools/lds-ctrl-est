# this supposedly shortens the long traceback
def pytest_exception_interact(node, call, report):
    excinfo = call.excinfo
    if 'example' in node.funcargs:
        excinfo.traceback.cut(path=node.funcargs['example'])
    report.longrepr = node.repr_failure(excinfo)

import matplotlib
matplotlib.use('Agg')