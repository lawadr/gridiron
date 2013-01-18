TEMPLATE = app
TARGET = gridiron

MOC_DIR = moc

INCLUDEPATH +=	include \
				$(OGRE_HOME)/include \
				$(OGRE_HOME)/include/OGRE

Debug:LIBS +=	-L$(OGRE_HOME)/bin/debug \
				-L$(OGRE_HOME)/lib/debug \
				-lOgreMain_d

Release:LIBS += -L$(OGRE_HOME)/bin/release \
				-L$(OGRE_HOME)/lib/release \
				-lOgreMain

HEADERS =	include/Angle.h \
			include/CameraController.h \
			include/Catalogue.h \
			include/CatalogueExplorer.h \
			include/Category.h \
			include/CategoryModel.h \
			include/Cell.h \
			include/EditorWindow.h \
			include/FloorTool.h \
			include/Grid.h \
			include/Item.h \
			include/Map.h \
			include/MapDocument.h \
			include/MapGeometry.h \
			include/MapReader.h \
			include/MapView.h \
			include/MapWriter.h \
			include/NewMapDialog.h \
			include/NewProjectDialog.h \
			include/NewTileDialog.h \
			include/NewTypeDialog.h \
			include/Object.h \
			include/ObjectTool.h \
			include/OgreWidget.h \
			include/PaintTool.h \
			include/PreviewBox.h \
			include/Project.h \
			include/PropertiesModel.h \
			include/Property.h \
			include/Region.h \
			include/ResourceModel.h \
			include/Scene.h \
			include/SceneEvent.h \
			include/SceneItem.h \
			include/SceneView.h \
			include/SelectionTool.h \
			include/Tile.h \
			include/TileSet.h \
			include/Tool.h \
			include/Type.h \
			include/ViewportGrid.h \
			include/Wall.h \
			include/WallTool.h

SOURCES =	src/CameraController.cpp \
			src/Catalogue.cpp \
			src/CatalogueExplorer.cpp \
			src/Category.cpp \
			src/CategoryModel.cpp \
			src/Editor.cpp \
			src/EditorWindow.cpp \
			src/FloorTool.cpp \
			src/Grid.cpp \
			src/Item.cpp \
			src/Map.cpp \
			src/MapDocument.cpp \
			src/MapGeometry.cpp \
			src/MapReader.cpp \
			src/MapView.cpp \
			src/MapWriter.cpp \
			src/NewMapDialog.cpp \
			src/NewProjectDialog.cpp \
			src/NewTileDialog.cpp \
			src/NewTypeDialog.cpp \
			src/Object.cpp \
			src/ObjectTool.cpp \
			src/OgreWidget.cpp \
			src/PaintTool.cpp \
			src/PreviewBox.cpp \
			src/Project.cpp \
			src/PropertiesModel.cpp \
			src/Region.cpp \
			src/ResourceModel.cpp \
			src/Scene.cpp \
			src/SceneEvent.cpp \
			src/SceneItem.cpp \
			src/SceneView.cpp \
			src/SelectionTool.cpp \
			src/Tile.cpp \
			src/TileSet.cpp \
			src/Tool.cpp \
			src/Type.cpp \
			src/ViewportGrid.cpp \
			src/Wall.cpp \
			src/WallTool.cpp
