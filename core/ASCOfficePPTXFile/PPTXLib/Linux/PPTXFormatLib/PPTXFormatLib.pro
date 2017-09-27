#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = PPTXFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../../Common/base.pri)

core_windows {
    QMAKE_CXXFLAGS_RELEASE += -Ob0
}

#BOOST
include($$PWD/../../../../Common/3dParty/boost/boost.pri)

DEFINES += UNICODE \
    _UNICODE \
    _USE_LIBXML2_READER_ \
    _USE_XMLLITE_READER_ \
    USE_LITE_READER \
    PPTX_DEF\
    PPT_DEF\
    ENABLE_PPT_TO_PPTX_CONVERT \
    AVS_USE_CONVERT_PPTX_TOCUSTOM_VML \
    #DISABLE_FILE_DOWNLOADER \
    CXIMAGE_DONT_DECLARE_TCHAR \
    DONT_WRITE_EMBEDDED_FONTS \
    CXIMAGE_DONT_DECLARE_TCHAR \
    LIBXML_READER_ENABLED

core_windows {
    INCLUDEPATH += ../../../../OfficeUtils/src/zlib-1.2.3
}

INCLUDEPATH += \
    ../../../../DesktopEditor/freetype-2.5.2/include \
    ../../../../Common/ASCDocxFormat/Source/Utility \
    ../../../../Common/ASCDocxFormat/Source/XML \
    ../../../../Common/ASCDocxFormat/Source \
    ../../../../DesktopEditor/xml/libxml2/include

SOURCES += pptxformatlib.cpp

core_release {
SOURCES += \
    pptxformatlib_logic.cpp
}

core_debug {
SOURCES += \
    ../../../PPTXFormat/Logic/Colors/SchemeClr.cpp \
    ../../../PPTXFormat/Logic/Fills/Blip.cpp \
    ../../../PPTXFormat/Logic/Table/TableCell.cpp \
    ../../../PPTXFormat/Logic/Timing/BuildNodeBase.cpp \
    ../../../PPTXFormat/Logic/Timing/TimeNodeBase.cpp \
    ../../../PPTXFormat/Logic/Transitions/TransitionBase.cpp \
    ../../../PPTXFormat/Logic/Bg.cpp \
    ../../../PPTXFormat/Logic/CNvGrpSpPr.cpp \
    ../../../PPTXFormat/Logic/ContentPart.cpp \
    ../../../PPTXFormat/Logic/CxnSp.cpp \
    ../../../PPTXFormat/Logic/EffectDag.cpp \
    ../../../PPTXFormat/Logic/EffectLst.cpp \
    ../../../PPTXFormat/Logic/EffectProperties.cpp \
    ../../../PPTXFormat/Logic/EffectStyle.cpp \
    ../../../PPTXFormat/Logic/Geometry.cpp \
    ../../../PPTXFormat/Logic/GraphicFrame.cpp \
    ../../../PPTXFormat/Logic/GrpSpPr.cpp \
    ../../../PPTXFormat/Logic/Hyperlink.cpp \
    ../../../PPTXFormat/Logic/NvGraphicFramePr.cpp \
    ../../../PPTXFormat/Logic/NvGrpSpPr.cpp \
    ../../../PPTXFormat/Logic/Pic.cpp \
    ../../../PPTXFormat/Logic/Shape.cpp \
    ../../../PPTXFormat/Logic/SmartArt.cpp \
    ../../../PPTXFormat/Logic/SpPr.cpp \
    ../../../PPTXFormat/Logic/SpTree.cpp \
    ../../../PPTXFormat/Logic/SpTreeElem.cpp \
    ../../../PPTXFormat/Logic/TxBody.cpp \
    ../../../PPTXFormat/Logic/UniColor.cpp \
    ../../../PPTXFormat/Logic/UniEffect.cpp \
    ../../../PPTXFormat/Logic/Runs/MathParaWrapper.cpp \
    ../../../PPTXFormat/Logic/Controls.cpp \
    ../../../PPTXFormat/FileContainer.cpp \
    ../../../PPTXFormat/FileFactory.cpp \
    ../../../PPTXFormat/FileMap.cpp \
    ../../../PPTXFormat/Folder.cpp
}

SOURCES += \
    ../../../ASCOfficeDrawingConverter.cpp \
    ../../../ASCOfficePPTXFileRealization.cpp \
    ../../../Editor/BinaryFileReaderWriter.cpp \
    ../../../Editor/FontPicker.cpp \
    ../../../Editor/Drawing/TextAttributesEx.cpp \
    ../../../Editor/Drawing/Elements.cpp \
    ../../../../HtmlRenderer/src/ASCSVGWriter.cpp

HEADERS += pptxformatlib.h \
    ../../../ASCOfficeDrawingConverter.h \
    ../../../ASCOfficePPTXFile.h \
    ../../../PPTXFormat/FileTypes.h \
    ../../../PPTXFormat/Namespaces.h \
    ../../../PPTXFormat/WritingVector.h \
    ../../../PPTXFormat/Limit/AlbumLayout.h \
    ../../../PPTXFormat/Limit/AnimationDgmBuild.h \
    ../../../PPTXFormat/Limit/BaseLimit.h \
    ../../../PPTXFormat/Limit/BevelType.h \
    ../../../PPTXFormat/Limit/BlendMode.h \
    ../../../PPTXFormat/Limit/BlipCompression.h \
    ../../../PPTXFormat/Limit/BWMode.h \
    ../../../PPTXFormat/Limit/CameraType.h \
    ../../../PPTXFormat/Limit/ChartBuild.h \
    ../../../PPTXFormat/Limit/ChartBuildType.h \
    ../../../PPTXFormat/Limit/ColorSchemeIndex.h \
    ../../../PPTXFormat/Limit/CompoundLine.h \
    ../../../PPTXFormat/Limit/Conformance.h \
    ../../../PPTXFormat/Limit/ContentStatus.h \
    ../../../PPTXFormat/Limit/CornerDirectionVal.h \
    ../../../PPTXFormat/Limit/DgmBuild.h \
    ../../../PPTXFormat/Limit/DgmBuildType.h \
    ../../../PPTXFormat/Limit/EffectContainerType.h \
    ../../../PPTXFormat/Limit/EightDirectionVal.h \
    ../../../PPTXFormat/Limit/FillPath.h \
    ../../../PPTXFormat/Limit/Flip.h \
    ../../../PPTXFormat/Limit/FontAlign.h \
    ../../../PPTXFormat/Limit/FontStyleIndex.h \
    ../../../PPTXFormat/Limit/FrameShape.h \
    ../../../PPTXFormat/Limit/HorzOverflow.h \
    ../../../PPTXFormat/Limit/InOutDirectionVal.h \
    ../../../PPTXFormat/Limit/IterateType.h \
    ../../../PPTXFormat/Limit/LastView.h \
    ../../../PPTXFormat/Limit/LightRigType.h \
    ../../../PPTXFormat/Limit/LineCap.h \
    ../../../PPTXFormat/Limit/LineEndSize.h \
    ../../../PPTXFormat/Limit/LineEndType.h \
    ../../../PPTXFormat/Limit/Material.h \
    ../../../PPTXFormat/Limit/OnOff.h \
    ../../../PPTXFormat/Limit/Orient.h \
    ../../../PPTXFormat/Limit/ParaBuildType.h \
    ../../../PPTXFormat/Limit/PathFillMode.h \
    ../../../PPTXFormat/Limit/PattFillVal.h \
    ../../../PPTXFormat/Limit/PenAlign.h \
    ../../../PPTXFormat/Limit/PlaceholderSize.h \
    ../../../PPTXFormat/Limit/PlaceholderType.h \
    ../../../PPTXFormat/Limit/PresetShadowVal.h \
    ../../../PPTXFormat/Limit/PrstClrVal.h \
    ../../../PPTXFormat/Limit/PrstDashVal.h \
    ../../../PPTXFormat/Limit/RectAlign.h \
    ../../../PPTXFormat/Limit/SchemeClrVal.h \
    ../../../PPTXFormat/Limit/ShapeType.h \
    ../../../PPTXFormat/Limit/SideDirectionVal.h \
    ../../../PPTXFormat/Limit/SlideLayoutType.h \
    ../../../PPTXFormat/Limit/SlideSize.h \
    ../../../PPTXFormat/Limit/SplitterBarState.h \
    ../../../PPTXFormat/Limit/SysClrVal.h \
    ../../../PPTXFormat/Limit/TextAlign.h \
    ../../../PPTXFormat/Limit/TextAnchor.h \
    ../../../PPTXFormat/Limit/TextAutonumberScheme.h \
    ../../../PPTXFormat/Limit/TextCaps.h \
    ../../../PPTXFormat/Limit/TextShapeType.h \
    ../../../PPTXFormat/Limit/TextStrike.h \
    ../../../PPTXFormat/Limit/TextTabAlignType.h \
    ../../../PPTXFormat/Limit/TextUnderline.h \
    ../../../PPTXFormat/Limit/TextVerticalType.h \
    ../../../PPTXFormat/Limit/TextWrap.h \
    ../../../PPTXFormat/Limit/TLAccumulate.h \
    ../../../PPTXFormat/Limit/TLAdditive.h \
    ../../../PPTXFormat/Limit/TLCalcMode.h \
    ../../../PPTXFormat/Limit/TLChartSubElement.h \
    ../../../PPTXFormat/Limit/TLColorDirection.h \
    ../../../PPTXFormat/Limit/TLColorSpace.h \
    ../../../PPTXFormat/Limit/TLCommandType.h \
    ../../../PPTXFormat/Limit/TLMasterRelation.h \
    ../../../PPTXFormat/Limit/TLNextAc.h \
    ../../../PPTXFormat/Limit/TLNodeFillType.h \
    ../../../PPTXFormat/Limit/TLNodeType.h \
    ../../../PPTXFormat/Limit/TLOrigin.h \
    ../../../PPTXFormat/Limit/TLOverride.h \
    ../../../PPTXFormat/Limit/TLPathEditMode.h \
    ../../../PPTXFormat/Limit/TLPresetClass.h \
    ../../../PPTXFormat/Limit/TLPrevAc.h \
    ../../../PPTXFormat/Limit/TLRestart.h \
    ../../../PPTXFormat/Limit/TLRuntimeTrigger.h \
    ../../../PPTXFormat/Limit/TLSyncBehavior.h \
    ../../../PPTXFormat/Limit/TLTransform.h \
    ../../../PPTXFormat/Limit/TLTransition.h \
    ../../../PPTXFormat/Limit/TLTriggerEvent.h \
    ../../../PPTXFormat/Limit/TLValueType.h \
    ../../../PPTXFormat/Limit/TransitionSpeed.h \
    ../../../PPTXFormat/Limit/VariantType.h \
    ../../../PPTXFormat/Limit/VertOverflow.h \
    ../../../PPTXFormat/Logic/Bullets/BuAutoNum.h \
    ../../../PPTXFormat/Logic/Bullets/BuBlip.h \
    ../../../PPTXFormat/Logic/Bullets/BuChar.h \
    ../../../PPTXFormat/Logic/Bullets/BuClr.h \
    ../../../PPTXFormat/Logic/Bullets/BuClrTx.h \
    ../../../PPTXFormat/Logic/Bullets/BuFontTx.h \
    ../../../PPTXFormat/Logic/Bullets/Bullet.h \
    ../../../PPTXFormat/Logic/Bullets/BulletColor.h \
    ../../../PPTXFormat/Logic/Bullets/BulletSize.h \
    ../../../PPTXFormat/Logic/Bullets/BulletTypeface.h \
    ../../../PPTXFormat/Logic/Bullets/BuNone.h \
    ../../../PPTXFormat/Logic/Bullets/BuSzPct.h \
    ../../../PPTXFormat/Logic/Bullets/BuSzPts.h \
    ../../../PPTXFormat/Logic/Bullets/BuSzTx.h \
    ../../../PPTXFormat/Logic/Colors/ColorBase.h \
    ../../../PPTXFormat/Logic/Colors/ColorModifier.h \
    ../../../PPTXFormat/Logic/Colors/PrstClr.h \
    ../../../PPTXFormat/Logic/Colors/SchemeClr.h \
    ../../../PPTXFormat/Logic/Colors/SrgbClr.h \
    ../../../PPTXFormat/Logic/Colors/SysClr.h \
    ../../../PPTXFormat/Logic/Effects/AlphaBiLevel.h \
    ../../../PPTXFormat/Logic/Effects/AlphaCeiling.h \
    ../../../PPTXFormat/Logic/Effects/AlphaFloor.h \
    ../../../PPTXFormat/Logic/Effects/AlphaInv.h \
    ../../../PPTXFormat/Logic/Effects/AlphaMod.h \
    ../../../PPTXFormat/Logic/Effects/AlphaModFix.h \
    ../../../PPTXFormat/Logic/Effects/AlphaOutset.h \
    ../../../PPTXFormat/Logic/Effects/AlphaRepl.h \
    ../../../PPTXFormat/Logic/Effects/BiLevel.h \
    ../../../PPTXFormat/Logic/Effects/Blend.h \
    ../../../PPTXFormat/Logic/Effects/Blur.h \
    ../../../PPTXFormat/Logic/Effects/ClrChange.h \
    ../../../PPTXFormat/Logic/Effects/ClrRepl.h \
    ../../../PPTXFormat/Logic/Effects/Duotone.h \
    ../../../PPTXFormat/Logic/Effects/EffectElement.h \
    ../../../PPTXFormat/Logic/Effects/FillEffect.h \
    ../../../PPTXFormat/Logic/Effects/FillOverlay.h \
    ../../../PPTXFormat/Logic/Effects/Glow.h \
    ../../../PPTXFormat/Logic/Effects/Grayscl.h \
    ../../../PPTXFormat/Logic/Effects/HslEffect.h \
    ../../../PPTXFormat/Logic/Effects/InnerShdw.h \
    ../../../PPTXFormat/Logic/Effects/LumEffect.h \
    ../../../PPTXFormat/Logic/Effects/OuterShdw.h \
    ../../../PPTXFormat/Logic/Effects/PrstShdw.h \
    ../../../PPTXFormat/Logic/Effects/Reflection.h \
    ../../../PPTXFormat/Logic/Effects/RelOff.h \
    ../../../PPTXFormat/Logic/Effects/SoftEdge.h \
    ../../../PPTXFormat/Logic/Effects/TintEffect.h \
    ../../../PPTXFormat/Logic/Effects/XfrmEffect.h \
    ../../../PPTXFormat/Logic/Fills/Blip.h \
    ../../../PPTXFormat/Logic/Fills/BlipFill.h \
    ../../../PPTXFormat/Logic/Fills/GradFill.h \
    ../../../PPTXFormat/Logic/Fills/NoFill.h \
    ../../../PPTXFormat/Logic/Fills/PattFill.h \
    ../../../PPTXFormat/Logic/Fills/SolidFill.h \
    ../../../PPTXFormat/Logic/Fills/Stretch.h \
    ../../../PPTXFormat/Logic/Fills/Tile.h \
    ../../../PPTXFormat/Logic/Media/AudioCD.h \
    ../../../PPTXFormat/Logic/Media/MediaFile.h \
    ../../../PPTXFormat/Logic/Media/WavAudioFile.h \
    ../../../PPTXFormat/Logic/Path2D/ArcTo.h \
    ../../../PPTXFormat/Logic/Path2D/Close.h \
    ../../../PPTXFormat/Logic/Path2D/CubicBezTo.h \
    ../../../PPTXFormat/Logic/Path2D/LineTo.h \
    ../../../PPTXFormat/Logic/Path2D/MoveTo.h \
    ../../../PPTXFormat/Logic/Path2D/PathBase.h \
    ../../../PPTXFormat/Logic/Path2D/QuadBezTo.h \
    ../../../PPTXFormat/Logic/Runs/Br.h \
    ../../../PPTXFormat/Logic/Runs/Fld.h \
    ../../../PPTXFormat/Logic/Runs/MathParaWrapper.h \
    ../../../PPTXFormat/Logic/Runs/Run.h \
    ../../../PPTXFormat/Logic/Runs/RunBase.h \
    ../../../PPTXFormat/Logic/Table/Table.h \
    ../../../PPTXFormat/Logic/Table/TableCell.h \
    ../../../PPTXFormat/Logic/Table/TableCellProperties.h \
    ../../../PPTXFormat/Logic/Table/TableCol.h \
    ../../../PPTXFormat/Logic/Table/TableProperties.h \
    ../../../PPTXFormat/Logic/Table/TableRow.h \
    ../../../PPTXFormat/Logic/Timing/Anim.h \
    ../../../PPTXFormat/Logic/Timing/AnimClr.h \
    ../../../PPTXFormat/Logic/Timing/AnimEffect.h \
    ../../../PPTXFormat/Logic/Timing/AnimMotion.h \
    ../../../PPTXFormat/Logic/Timing/AnimRot.h \
    ../../../PPTXFormat/Logic/Timing/AnimScale.h \
    ../../../PPTXFormat/Logic/Timing/AnimVariant.h \
    ../../../PPTXFormat/Logic/Timing/AttrName.h \
    ../../../PPTXFormat/Logic/Timing/AttrNameLst.h \
    ../../../PPTXFormat/Logic/Timing/Audio.h \
    ../../../PPTXFormat/Logic/Timing/BldDgm.h \
    ../../../PPTXFormat/Logic/Timing/BldGraphic.h \
    ../../../PPTXFormat/Logic/Timing/BldLst.h \
    ../../../PPTXFormat/Logic/Timing/BldOleChart.h \
    ../../../PPTXFormat/Logic/Timing/BldP.h \
    ../../../PPTXFormat/Logic/Timing/BldSub.h \
    ../../../PPTXFormat/Logic/Timing/BuildNodeBase.h \
    ../../../PPTXFormat/Logic/Timing/CBhvr.h \
    ../../../PPTXFormat/Logic/Timing/Cmd.h \
    ../../../PPTXFormat/Logic/Timing/CMediaNode.h \
    ../../../PPTXFormat/Logic/Timing/Cond.h \
    ../../../PPTXFormat/Logic/Timing/CondLst.h \
    ../../../PPTXFormat/Logic/Timing/CTn.h \
    ../../../PPTXFormat/Logic/Timing/Excl.h \
    ../../../PPTXFormat/Logic/Timing/GraphicEl.h \
    ../../../PPTXFormat/Logic/Timing/Iterate.h \
    ../../../PPTXFormat/Logic/Timing/Par.h \
    ../../../PPTXFormat/Logic/Timing/Seq.h \
    ../../../PPTXFormat/Logic/Timing/Set.h \
    ../../../PPTXFormat/Logic/Timing/SpTgt.h \
    ../../../PPTXFormat/Logic/Timing/Tav.h \
    ../../../PPTXFormat/Logic/Timing/TavLst.h \
    ../../../PPTXFormat/Logic/Timing/TgtEl.h \
    ../../../PPTXFormat/Logic/Timing/TimeNodeBase.h \
    ../../../PPTXFormat/Logic/Timing/Timing.h \
    ../../../PPTXFormat/Logic/Timing/Tmpl.h \
    ../../../PPTXFormat/Logic/Timing/TmplLst.h \
    ../../../PPTXFormat/Logic/Timing/TnLst.h \
    ../../../PPTXFormat/Logic/Timing/TxEl.h \
    ../../../PPTXFormat/Logic/Timing/Video.h \
    ../../../PPTXFormat/Logic/Transitions/CornerDirectionTransition.h \
    ../../../PPTXFormat/Logic/Transitions/EightDirectionTransition.h \
    ../../../PPTXFormat/Logic/Transitions/EmptyTransition.h \
    ../../../PPTXFormat/Logic/Transitions/OptionalBlackTransition.h \
    ../../../PPTXFormat/Logic/Transitions/OrientationTransition.h \
    ../../../PPTXFormat/Logic/Transitions/SideDirectionTransition.h \
    ../../../PPTXFormat/Logic/Transitions/SndAc.h \
    ../../../PPTXFormat/Logic/Transitions/SplitTransition.h \
    ../../../PPTXFormat/Logic/Transitions/StSnd.h \
    ../../../PPTXFormat/Logic/Transitions/Transition.h \
    ../../../PPTXFormat/Logic/Transitions/TransitionBase.h \
    ../../../PPTXFormat/Logic/Transitions/WheelTransition.h \
    ../../../PPTXFormat/Logic/Transitions/ZoomTransition.h \
    ../../../PPTXFormat/Logic/Controls.h \
    ../../../PPTXFormat/Logic/Ah.h \
    ../../../PPTXFormat/Logic/AhBase.h \
    ../../../PPTXFormat/Logic/AhPolar.h \
    ../../../PPTXFormat/Logic/AhXY.h \
    ../../../PPTXFormat/Logic/Backdrop.h \
    ../../../PPTXFormat/Logic/Bevel.h \
    ../../../PPTXFormat/Logic/Bg.h \
    ../../../PPTXFormat/Logic/BgPr.h \
    ../../../PPTXFormat/Logic/BodyPr.h \
    ../../../PPTXFormat/Logic/Camera.h \
    ../../../PPTXFormat/Logic/Cell3D.h \
    ../../../PPTXFormat/Logic/ClrMap.h \
    ../../../PPTXFormat/Logic/ClrMapOvr.h \
    ../../../PPTXFormat/Logic/CNvCxnSpPr.h \
    ../../../PPTXFormat/Logic/CNvGraphicFramePr.h \
    ../../../PPTXFormat/Logic/CNvGrpSpPr.h \
    ../../../PPTXFormat/Logic/CNvPicPr.h \
    ../../../PPTXFormat/Logic/CNvPr.h \
    ../../../PPTXFormat/Logic/CNvSpPr.h \
    ../../../PPTXFormat/Logic/ContentPart.h \
    ../../../PPTXFormat/Logic/CSld.h \
    ../../../PPTXFormat/Logic/CustGeom.h \
    ../../../PPTXFormat/Logic/Cxn.h \
    ../../../PPTXFormat/Logic/CxnSp.h \
    ../../../PPTXFormat/Logic/DefaultShapeDefinition.h \
    ../../../PPTXFormat/Logic/EffectDag.h \
    ../../../PPTXFormat/Logic/EffectLst.h \
    ../../../PPTXFormat/Logic/EffectProperties.h \
    ../../../PPTXFormat/Logic/EffectStyle.h \
    ../../../PPTXFormat/Logic/ExtP.h \
    ../../../PPTXFormat/Logic/FillStyle.h \
    ../../../PPTXFormat/Logic/FontCollection.h \
    ../../../PPTXFormat/Logic/FontRef.h \
    ../../../PPTXFormat/Logic/Gd.h \
    ../../../PPTXFormat/Logic/Geometry.h \
    ../../../PPTXFormat/Logic/GraphicFrame.h \
    ../../../PPTXFormat/Logic/GrpSpPr.h \
    ../../../PPTXFormat/Logic/Gs.h \
    ../../../PPTXFormat/Logic/HeadingVariant.h \
    ../../../PPTXFormat/Logic/Hf.h \
    ../../../PPTXFormat/Logic/Hyperlink.h \
    ../../../PPTXFormat/Logic/LightRig.h \
    ../../../PPTXFormat/Logic/Lin.h \
    ../../../PPTXFormat/Logic/LineEnd.h \
    ../../../PPTXFormat/Logic/LineJoin.h \
    ../../../PPTXFormat/Logic/LineStyle.h \
    ../../../PPTXFormat/Logic/Ln.h \
    ../../../PPTXFormat/Logic/NvCxnSpPr.h \
    ../../../PPTXFormat/Logic/NvGraphicFramePr.h \
    ../../../PPTXFormat/Logic/NvGrpSpPr.h \
    ../../../PPTXFormat/Logic/NvPicPr.h \
    ../../../PPTXFormat/Logic/NvPr.h \
    ../../../PPTXFormat/Logic/NvSpPr.h \
    ../../../PPTXFormat/Logic/Paragraph.h \
    ../../../PPTXFormat/Logic/PartTitle.h \
    ../../../PPTXFormat/Logic/Path.h \
    ../../../PPTXFormat/Logic/Path2D.h \
    ../../../PPTXFormat/Logic/Ph.h \
    ../../../PPTXFormat/Logic/Pic.h \
    ../../../PPTXFormat/Logic/PrstDash.h \
    ../../../PPTXFormat/Logic/PrstGeom.h \
    ../../../PPTXFormat/Logic/PrstTxWarp.h \
    ../../../PPTXFormat/Logic/Rect.h \
    ../../../PPTXFormat/Logic/Rot.h \
    ../../../PPTXFormat/Logic/RunElem.h \
    ../../../PPTXFormat/Logic/RunProperties.h \
    ../../../PPTXFormat/Logic/Scene3d.h \
    ../../../PPTXFormat/Logic/Shape.h \
    ../../../PPTXFormat/Logic/ShapeStyle.h \
    ../../../PPTXFormat/Logic/SmartArt.h \
    ../../../PPTXFormat/Logic/Sp3d.h \
    ../../../PPTXFormat/Logic/SpPr.h \
    ../../../PPTXFormat/Logic/SpTree.h \
    ../../../PPTXFormat/Logic/SpTreeElem.h \
    ../../../PPTXFormat/Logic/StyleRef.h \
    ../../../PPTXFormat/Logic/SupplementalFont.h \
    ../../../PPTXFormat/Logic/Tab.h \
    ../../../PPTXFormat/Logic/TableBgStyle.h \
    ../../../PPTXFormat/Logic/TablePartStyle.h \
    ../../../PPTXFormat/Logic/TableStyle.h \
    ../../../PPTXFormat/Logic/TcBdr.h \
    ../../../PPTXFormat/Logic/TcStyle.h \
    ../../../PPTXFormat/Logic/TcTxStyle.h \
    ../../../PPTXFormat/Logic/TextFit.h \
    ../../../PPTXFormat/Logic/TextFont.h \
    ../../../PPTXFormat/Logic/TextListStyle.h \
    ../../../PPTXFormat/Logic/TextParagraphPr.h \
    ../../../PPTXFormat/Logic/TextSpacing.h \
    ../../../PPTXFormat/Logic/Timing.h \
    ../../../PPTXFormat/Logic/TxBody.h \
    ../../../PPTXFormat/Logic/TxStyles.h \
    ../../../PPTXFormat/Logic/UniColor.h \
    ../../../PPTXFormat/Logic/UniEffect.h \
    ../../../PPTXFormat/Logic/UniFill.h \
    ../../../PPTXFormat/Logic/UniMedia.h \
    ../../../PPTXFormat/Logic/UniPath2D.h \
    ../../../PPTXFormat/Logic/Xfrm.h \
    ../../../PPTXFormat/Logic/XmlId.h \
    ../../../PPTXFormat/Presentation/EmbeddedFont.h \
    ../../../PPTXFormat/Presentation/EmbeddedFontDataId.h \
    ../../../PPTXFormat/Presentation/Kinsoku.h \
    ../../../PPTXFormat/Presentation/NotesSz.h \
    ../../../PPTXFormat/Presentation/PhotoAlbum.h \
    ../../../PPTXFormat/Presentation/SldSz.h \
    ../../../PPTXFormat/Theme/ClrScheme.h \
    ../../../PPTXFormat/Theme/ExtraClrScheme.h \
    ../../../PPTXFormat/Theme/FmtScheme.h \
    ../../../PPTXFormat/Theme/FontScheme.h \
    ../../../PPTXFormat/Theme/ThemeElements.h \
    ../../../PPTXFormat/ViewProps/CSldViewPr.h \
    ../../../PPTXFormat/ViewProps/CViewPr.h \
    ../../../PPTXFormat/ViewProps/GridSpacing.h \
    ../../../PPTXFormat/ViewProps/Guide.h \
    ../../../PPTXFormat/ViewProps/NormalViewPr.h \
    ../../../PPTXFormat/ViewProps/NotesTextViewPr.h \
    ../../../PPTXFormat/ViewProps/NotesViewPr.h \
    ../../../PPTXFormat/ViewProps/Origin.h \
    ../../../PPTXFormat/ViewProps/OutlineViewPr.h \
    ../../../PPTXFormat/ViewProps/Ratio.h \
    ../../../PPTXFormat/ViewProps/Restored.h \
    ../../../PPTXFormat/ViewProps/Scale.h \
    ../../../PPTXFormat/ViewProps/Sld.h \
    ../../../PPTXFormat/ViewProps/SlideViewPr.h \
    ../../../PPTXFormat/ViewProps/SorterViewPr.h \
    ../../../PPTXFormat/App.h \
    ../../../PPTXFormat/CommentAuthors.h \
    ../../../PPTXFormat/Comments.h \
    ../../../PPTXFormat/LegacyDiagramText.h \
    ../../../PPTXFormat/Core.h \
    ../../../PPTXFormat/FileContainer.h \
    ../../../PPTXFormat/FileFactory.h \
    ../../../PPTXFormat/FileMap.h \
    ../../../PPTXFormat/Folder.h \
    ../../../PPTXFormat/HandoutMaster.h \
    ../../../PPTXFormat/NotesMaster.h \
    ../../../PPTXFormat/NotesSlide.h \
    ../../../PPTXFormat/PPTX.h \
    ../../../PPTXFormat/PPTXEvent.h \
    ../../../PPTXFormat/Presentation.h \
    ../../../PPTXFormat/PresProps.h \
    ../../../PPTXFormat/Slide.h \
    ../../../PPTXFormat/SlideLayout.h \
    ../../../PPTXFormat/SlideMaster.h \
    ../../../PPTXFormat/TableStyles.h \
    ../../../PPTXFormat/Theme.h \
    ../../../PPTXFormat/ViewProps.h \
    ../../../PPTXFormat/WrapperFile.h \
    ../../../PPTXFormat/WrapperWritingElement.h \
    ../../../Editor/BinaryFileReaderWriter.h \
    ../../../Editor/BinReaderWriterDefines.h \
    ../../../Editor/BinWriters.h \
    ../../../Editor/CalculatorCRC32.h \
    ../../../Editor/Converter.h \
    ../../../Editor/FontCutter.h \
    ../../../Editor/FontPicker.h \
    ../../../Editor/imagemanager.h \
    ../../../Editor/PPTXWriter.h \
    ../../../Editor/WMFToImageConverter.h \
    ../../../Editor/PresentationDrawingsDef.h \
    ../../../Editor/DefaultNotesMaster.h \
    ../../../Editor/DefaultNotesTheme.h \
    ../../../../Common/DocxFormat/Source/Base/Nullable.h \
    ../../../../HtmlRenderer/include/ASCSVGWriter.h \
    ../../../../Common/FileDownloader/FileDownloader.h \
    ../../../PPTXFormat/ShowPr/Browse.h \
    ../../../PPTXFormat/ShowPr/CustShow.h \
    ../../../PPTXFormat/ShowPr/Kiosk.h \
    ../../../PPTXFormat/ShowPr/Present.h \
    ../../../PPTXFormat/ShowPr/ShowPr.h \
    ../../../PPTXFormat/ShowPr/SldAll.h \
    ../../../PPTXFormat/ShowPr/SldRg.h

core_windows {
    SOURCES += \
        ../../../../Common/FileDownloader/FileDownloader_win.cpp
}
core_linux {
    SOURCES += \
        ../../../../Common/FileDownloader/FileDownloader_curl.cpp
}
core_mac {
    OBJECTIVE_SOURCES += \
        ../../../../Common/FileDownloader/FileDownloader_mac.mm
}
