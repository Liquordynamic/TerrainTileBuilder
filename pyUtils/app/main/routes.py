import os
from ..main import bp
from flask import request, jsonify, abort, Response

ROOT_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), '../', '../', '../')
RESOURCE_PATH = os.path.join(ROOT_PATH, 'resource')

######################################## Error Handler ########################################

@bp.errorhandler(400)
def not_found(error):
    
    response = jsonify({
        'status': 400,
        'error': 'Bad Request',
        'message': error.description
    })
    return response, 400

@bp.errorhandler(404)
def not_found(error):
    
    response = jsonify({
        'status': 404,
        'error': 'Not Found',
        'message': error.description
    })
    return response, 404

######################################## API for File System ########################################

@bp.route('/v0/terrain-rgb/<z>/<x>/<y>.png', methods=[ 'GET' ])
def get_terrain_tile(x: int, y: int, z: int):

    png_path = os.path.join(RESOURCE_PATH, 'output', f'{z}', f'{x}-{y}-{z}.png')
    if os.path.exists(png_path):
        return Response(open(png_path, 'rb'), mimetype='image/png')
    else:
        abort(404, description='Terrain Tile Not Found')
