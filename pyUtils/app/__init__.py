from flask import Flask
    
def create_app():
    
    app = Flask('Terrain Tile Provider')

    from .main import bp as main_bp
    app.register_blueprint(main_bp)

    return app
