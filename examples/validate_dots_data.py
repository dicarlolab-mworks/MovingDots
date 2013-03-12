import numpy

from mworks.data import MWKFile


def validate_dots_data(filename):
    with MWKFile(filename) as f:
        num_events = 0
        for e in f.get_events_iter(codes=['#announceStimulus']):
            value = e.value
            if (isinstance(value, dict) and
                (value['type'] == 'moving_dots') and
                ('dots' in value)):
                data = numpy.fromstring(value['dots'], numpy.float32)
                assert len(data) == (2 * value['num_dots'])
                
                x = data[::2] - value['field_center_x']
                y = data[1::2] - value['field_center_y']
                field_radius = value['field_radius']
                assert (x*x + y*y).max() <= (field_radius * field_radius)
                
                num_events += 1

        assert num_events > 0
        print 'Processed %d events' % num_events


if __name__ == '__main__':
    import sys
    validate_dots_data(sys.argv[1])
