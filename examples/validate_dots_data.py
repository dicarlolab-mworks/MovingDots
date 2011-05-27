import numpy

from mworks.data import MWKFile


NUM_DOTS = 1000
FIELD_RADIUS = 10


def validate_dots_data(filename):
    f = MWKFile(filename)
    f.open()
    try:
        events = f.get_events(codes=['#announceStimulus'])
        for i, data in enumerate(e.value['dots'] for e in events
                                 if isinstance(e.value, dict) and
                                 e.value['type'] == 'dynamic_random_dots'):
            data = numpy.fromstring(data, numpy.float32)
            assert len(data) == (2 * NUM_DOTS)
            x = data[::2]
            y = data[1::2]
            assert (x*x + y*y).max() <= (FIELD_RADIUS * FIELD_RADIUS)

        assert i > 0
        print 'Processed %d events' % i

    finally:
        f.close()
        f.unindex()


if __name__ == '__main__':
    import sys
    validate_dots_data(sys.argv[1])
